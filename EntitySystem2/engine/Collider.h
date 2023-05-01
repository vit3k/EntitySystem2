#pragma once

#include "EntityW/Entity.h"
#include "Logger.h"
#include "Vector2.h"
#include <cmath>
#include <vector>

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
	Collision sat(EntityW::EntitySp entity1, EntityW::EntitySp entity2, std::vector<Vector2> axes);
public:
	virtual Collision collide(EntityW::EntitySp entity1, EntityW::EntitySp entity2) = 0;
};

class BoxBoxCollider : public Collider {
	Logger logger = Logger::get("BoxBoxCollider");
public:
	virtual Collision collide(EntityW::EntitySp entity1, EntityW::EntitySp entity2);
};

class BoxCircleCollider : public Collider {
	Logger logger = Logger::get("BoxCircleCollider");
public:
	virtual Collision collide(EntityW::EntitySp entity1, EntityW::EntitySp entity2);
};

