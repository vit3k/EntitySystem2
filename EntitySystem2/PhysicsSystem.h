#pragma once
#include "EntityW\System.h"
#include "Components.h"
#include "EntityW\Time.h"
#include "CollisionSystem.h"

class PhysicsSystem : public EntityW::System<TransformComponent, VelocityComponent, PhysicsComponent>
{
	std::vector<CollisionEventSp> collisions;
	Logger logger = Logger::get("PS");
	void correctEntity(EntityW::EntitySp entity, Collision& collision, float prop);
	void bounceEntity(EntityW::EntitySp entity);
	float getMass(EntityW::EntitySp entity);
public:
	virtual void Process(EntityW::Time deltaTime);
	void OnCollision(EventSp e);
	PhysicsSystem();
};