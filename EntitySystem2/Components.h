#pragma once
#include "EntityW\Component.h"
#include <SFML/Graphics.hpp>
#include "Vector2.h"
#include <glm.hpp>
#include "EntityW\Entity.h"
#include "Collider.h"

class TransformComponent : public EntityW::Component {
public:
	Vector2 position;
	TransformComponent(Vector2 position) : position(position) {}
	
};

typedef std::shared_ptr<TransformComponent> TransformComponentSp;

class VelocityComponent : public EntityW::Component {
public:
	Vector2 velocity;
	float bounciness;
	VelocityComponent(Vector2 velocity, float bounciness) : velocity(velocity), bounciness(bounciness) {}
};

class InputComponent : public EntityW::Component
{
public:
	int controller;
	InputComponent(int controller) : controller(controller) {};
};

class RenderComponent : public EntityW::Component {
public:
	sf::Shape* shape;
	RenderComponent(sf::Shape* shape) : shape(shape) {};
};

class TextComponent : public EntityW::Component {
public:
	std::string text;
	TextComponent(std::string text) : text(text) {};
};
enum CollisionShapeType {
	Circle, Rect
};

struct Projection {
	float min, max;
	Projection(float min, float max) : min(min), max(max) {}
	float overlaps(Projection& other) {
		return fmax(0, fmin(max, other.max) - fmax(min, other.min));
	}
};

class CollisionShape
{
public:
	virtual CollisionShapeType getType() = 0;
	virtual Vector2 center() = 0;
	virtual Projection project(TransformComponentSp transform, Vector2 axis) = 0;
};

class RectCollisionShape : public CollisionShape
{
public:
	float width, height;
	virtual CollisionShapeType getType() { return Rect; }
	RectCollisionShape(float width, float height) : width(width), height(height) {}
	std::vector<Vector2> calculateVertices(TransformComponentSp transform) {
		std::vector<Vector2> vertices = { Vector2(transform->position.x, transform->position.y),
			Vector2(transform->position.x + width, transform->position.y),
			Vector2(transform->position.x + width, transform->position.y + height),
			Vector2(transform->position.x, transform->position.y + height) };

		return vertices;
	}
	virtual Vector2 center() {
		return Vector2(width / 2, height / 2);
	}
	virtual Projection project(TransformComponentSp transform, Vector2 axis) {
		
		auto vertices = calculateVertices(transform);

		float max = glm::dot(axis, vertices[0]);

		float min = max;

		for (int i = 1; i < 4; i++)
		{
			float p = glm::dot(axis, vertices[i]);
			if (p < min)
			{
				min = p;
			}
			else if (p > max)
			{
				max = p;
			}
		}
		return Projection(min, max);
	}
};

class CircleCollisionShape : public CollisionShape
{
public:
	float radius;
	virtual CollisionShapeType getType() { return Circle; }
	CircleCollisionShape(float radius) : radius(radius) {}
	virtual Vector2 center() {
		return Vector2(radius, radius);
	}
	virtual Projection project(TransformComponentSp transform, Vector2 axis) {
		auto projected = glm::dot(axis, transform->position + center());
		return Projection(projected - radius, projected + radius);
	}
};

class CollisionComponent : public EntityW::Component {
public:
	Collision collision;
	CollisionShape* shape;
	CollisionComponent(CollisionShape* shape) : shape(shape) {};
};

class PhysicsComponent : public EntityW::Component {
public:
	Vector2 constraints;
	float bounciness;
	float mass;
	float invertedMass;
	PhysicsComponent(float bounciness, float mass, Vector2 constraints) : bounciness(bounciness), mass(mass), constraints(constraints)
	{
		if (mass != 0) {
			invertedMass = 1 / mass;
		}
		else
		{
			invertedMass = 0;
		}
	}
};

class ScoringSurfaceComponent : public EntityW::Component {
public:
	int playerId;
	EntityW::EntitySp paddle;
	ScoringSurfaceComponent(int playerId, EntityW::EntitySp paddle) : playerId(playerId), paddle(paddle) {}
};

class AttachComponent : public EntityW::Component {
public:
	TransformComponentSp parentTransform;
	Vector2 relativePosition;
	AttachComponent(TransformComponentSp parentTransform, Vector2 relativePosition)
		: parentTransform(parentTransform), relativePosition(relativePosition) {

	}
};