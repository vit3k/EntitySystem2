#pragma once

#include "EntityW\Entity.h"
#include "Logger.h"
#include "Vector2.h"
#include <cmath>
#include <vector>

struct Projection {
	float min, max;
	Projection(float min, float max) : min(min), max(max) {}
	float overlaps(Projection& other) {
		return fmax(0, fmin(max, other.max) - fmax(min, other.min));
	}
};

class Collision {
	
public:
	bool occured = false;
	float depth;
	Vector2 normal;
	Collision(bool occured) : occured(occured) {}
	Collision() : occured(false) {};
};

class Collider
{
	Logger logger = Logger::get("Collider");
protected:
	Projection project(EntityW::EntitySp entity, Vector2 axis);
	std::vector<Vector2> calculateVertices(EntityW::EntitySp entity);
public:
	virtual Collision collide(EntityW::EntitySp entity1, EntityW::EntitySp entity2) = 0;
};

class BoxBoxCollider : public Collider {
	Logger logger = Logger::get("BoxBoxCollider");
public:
	virtual Collision collide(EntityW::EntitySp entity1, EntityW::EntitySp entity2);
};

class BoxCircleCollider : public Collider {
public:
	virtual Collision collide(EntityW::EntitySp entity1, EntityW::EntitySp entity2);
};

