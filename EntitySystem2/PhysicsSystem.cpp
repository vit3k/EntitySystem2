#include "PhysicsSystem.h"
#include "EventDispatcher.h"
#include "EntityW\Entity.h"
#include <sstream>

using namespace EntityW;

void PhysicsSystem::Process(EntityW::Time deltaTime)
{
	for (auto collisionEvent : collisions)
	{
		//logger.log("Collision: " + Vector2Utils::toString(collisionEvent->collision.normal)+" "+std::to_string(collisionEvent->collision.depth));
		auto entity1 = collisionEvent->entity1;
		auto entity2 = collisionEvent->entity2;
		
		float entity1Mass = getMass(entity1);
		float entity2Mass = getMass(entity2);
		float entity1MassProp = 1-(entity1Mass / (entity1Mass + entity2Mass));
		float entity2MassProp = 1 - (entity2Mass / (entity1Mass + entity2Mass));

		Vector2 entity1PropVector = calculatePropVector(entity1, entity2, entity1MassProp);
		Vector2 entity2PropVector = -calculatePropVector(entity2, entity1, entity2MassProp);
		correctEntity(entity1, collisionEvent->collision, entity1PropVector);
		correctEntity(entity2, collisionEvent->collision, entity2PropVector);

		bounceEntity(entity1, collisionEvent->collision, entity1PropVector);
		bounceEntity(entity2, collisionEvent->collision, entity2PropVector);

	}
	collisions.clear();
}
Vector2 PhysicsSystem::calculatePropVector(EntityW::EntitySp entity, EntityW::EntitySp otherEntity, float massProp) {
	
	Vector2 otherConstraints = getConstraints(otherEntity);
	Vector2 constraints = getConstraints(entity);
	Vector2 prop;

	if (otherConstraints.x == 0) {
		prop.x = 1;
	}
	else {
		prop.x = massProp * constraints.x;
	}

	if (otherConstraints.y == 0) {
		prop.y = 1;
	}
	else {
		prop.y = massProp * constraints.y;
	}
	//logger.log(Vector2Utils::toString(constraints) + " " + Vector2Utils::toString(otherConstraints) + " " + Vector2Utils::toString(prop));
	return prop;
}

Vector2 PhysicsSystem::getConstraints(EntityW::EntitySp entity) {
	if (entity->has<PhysicsComponent>())
	{
		return entity->get<PhysicsComponent>()->constraints;
	}
	return Vector2(0,0);
}
float PhysicsSystem::getMass(EntityW::EntitySp entity)
{
	if (entity->has<PhysicsComponent>())
	{
		return entity->get<PhysicsComponent>()->mass;
	}
	return 0.;
}

void PhysicsSystem::bounceEntity(EntitySp entity, Collision& collision, Vector2 prop)
{
	if (entity->has<VelocityComponent>() && entity->has<PhysicsComponent>())
	{
		auto physics = entity->get<PhysicsComponent>();
		auto velocity = entity->get<VelocityComponent>();
		
		velocity->velocity = velocity->velocity - 2 * glm::dot(velocity->velocity, collision.normal) * collision.normal;

		logger.log("B "+Vector2Utils::toString(collision.normal) + " " + Vector2Utils::toString(velocity->velocity));
	}
}

void PhysicsSystem::correctEntity(EntityW::EntitySp entity, Collision& collision, Vector2 prop)
{
	
	auto transform = entity->get<TransformComponent>();
	auto correction = collision.normal * collision.depth;
	correction.x *= prop.x;
	correction.y *= prop.y;
	logger.log("C "+Vector2Utils::toString(prop)+ " "+Vector2Utils::toString(correction));
	transform->position += correction;
	
}

PhysicsSystem::PhysicsSystem() 
{
	EventDispatcher::get().subscribe<CollisionEvent>(EventListenerDelegate(this, &PhysicsSystem::OnCollision));
}

void PhysicsSystem::OnCollision(EventSp e)
{
	auto collisionEvent = std::dynamic_pointer_cast<CollisionEvent>(e);
	collisions.push_back(collisionEvent);
}