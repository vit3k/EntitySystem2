#pragma once
#include "EntityW\System.h"
#include "Components.h"
#include "EntityW\Time.h"
#include "CollisionSystem.h"

class PhysicsSystem : public EntityW::System<TransformComponent, VelocityComponent, PhysicsComponent>
{
	std::vector<CollisionEventSp> collisions;
	Logger logger = Logger::get("PhysicsSystem");
	void correctEntity(EntityW::EntitySp entity, Collision& collision, Vector2 prop);
	void bounceEntity(EntityW::EntitySp entity, Collision& collision, Vector2 prop, Vector2 otherVelocity);
	float getMass(EntityW::EntitySp entity);
	Vector2 getVelocity(EntityW::EntitySp entity);
	Vector2 calculatePropVector(EntityW::EntitySp entity, EntityW::EntitySp otherEntity, float massProp);
	Vector2 getConstraints(EntityW::EntitySp entity);
public:
	virtual void Process(EntityW::Time deltaTime);
	void OnCollision(EventSp e);
	PhysicsSystem();
};