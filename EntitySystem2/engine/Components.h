#pragma once
#include "EntityW/Component.h"
#include <SFML/Graphics.hpp>
#include "Vector2.h"
#include <glm/glm.hpp>
#include "EntityW/Entity.h"
#include "Collider.h"

enum ShapeType {
	Rectangle, Circle
};

class TransformComponent : public EntityW::Component<TransformComponent> {
public:
	Vector2 position;

	TransformComponent(Vector2 position) : position(position) {}
	static std::shared_ptr<TransformComponent> create(Vector2 position) {
		return std::make_shared<TransformComponent>(position);
	}
};

typedef std::shared_ptr<TransformComponent> TransformComponentSp;

class VelocityComponent : public EntityW::Component<VelocityComponent> {
public:
	Vector2 velocity;
	float bounciness;
	VelocityComponent(Vector2 velocity, float bounciness) : velocity(velocity), bounciness(bounciness) {}
};

class RenderComponent : public EntityW::Component<RenderComponent> {
public:
	std::shared_ptr<sf::Shape> shape;
	RenderComponent(std::shared_ptr<sf::Shape> shape) : shape(shape) {};
};

class TextComponent : public EntityW::Component<TextComponent> {
public:
	std::string text;
	TextComponent(std::string text) : text(text) {};
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
	virtual ShapeType getType() = 0;
	virtual Vector2 center() = 0;
	virtual Projection project(TransformComponentSp transform, Vector2 axis) = 0;
};

class RectCollisionShape : public CollisionShape
{
public:
	float width, height;
	virtual ShapeType getType() { return Rectangle; }
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

	~RectCollisionShape()
	{
		std::cout << "RectCollisionShape destroyed" << std::endl;
	}
};

class CircleCollisionShape : public CollisionShape
{
public:
	float radius;
	virtual ShapeType getType() { return Circle; }
	CircleCollisionShape(float radius) : radius(radius) {}
	virtual Vector2 center() {
		return Vector2(radius, radius);
	}
	virtual Projection project(TransformComponentSp transform, Vector2 axis) {
		auto projected = glm::dot(axis, transform->position + center());
		return Projection(projected - radius, projected + radius);
	}
};

class CollisionComponent : public EntityW::Component<CollisionComponent> {
public:
	Collision collision;
	std::shared_ptr<CollisionShape> shape;
	CollisionComponent(std::shared_ptr<CollisionShape> shape) : shape(shape) {}

};

class PhysicsComponent : public EntityW::Component<PhysicsComponent> {
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
