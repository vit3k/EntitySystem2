#pragma once
#include "EntityW\Component.h"
#include <SFML/Graphics.hpp>
#include "Vector2.h"
#include <glm.hpp>

class TransformComponent : public EntityW::Component {
public:
	Vector2 position;
	TransformComponent(Vector2 position) : position(position) {}
	
};

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

class CollisionShape
{
public:
	virtual CollisionShapeType getType() = 0;
	virtual Vector2 center() = 0;
};

class RectCollisionShape : public CollisionShape
{
public:
	float width, height;
	virtual CollisionShapeType getType() { return Rect; }
	RectCollisionShape(float width, float height) : width(width), height(height) {}
	virtual Vector2 center() {
		return Vector2(width / 2, height / 2);
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
};

class CollisionComponent : public EntityW::Component {
public:
	CollisionShape* shape;
	CollisionComponent(CollisionShape* shape) : shape(shape) {};
};

class PhysicsComponent : public EntityW::Component {
public:
	//Vector2 constraints;
	float bounciness;
	float mass;
	float invertedMass;
	PhysicsComponent(float bounciness, float mass) : bounciness(bounciness), mass(mass) 
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