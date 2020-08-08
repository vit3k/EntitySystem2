#include "PhysicsSystem.h"
#include "EntityW/EventDispatcher.h"
#include "EntityW/Entity.h"
#include <sstream>

using namespace EntityW;

void PhysicsSystem::Process(EntityW::Time deltaTime)
{
	for (auto collisionEvent : collisions)
	{

		auto entity1 = collisionEvent->entity1;
		auto entity2 = collisionEvent->entity2;

		if (entity1->has<PhysicsComponent>() && entity2->has<PhysicsComponent>()) {
			float entity1Mass = getMass(entity1);
			float entity2Mass = getMass(entity2);
			float entity1MassProp = 1 - (entity1Mass / (entity1Mass + entity2Mass));
			float entity2MassProp = 1 - (entity2Mass / (entity1Mass + entity2Mass));

			Vector2 entity1PropVector = calculatePropVector(entity1, entity2, entity1MassProp);
			Vector2 entity2PropVector = calculatePropVector(entity2, entity1, entity2MassProp);
			correctEntity(entity1, collisionEvent->collision, entity1PropVector, 1);
			correctEntity(entity2, collisionEvent->collision, entity2PropVector, -1);
			/*if (entity1->has<VelocityComponent>())
			{
				entity1->get<VelocityComponent>()->velocity = Vector2(0, 0);
			}
			if (entity2->has<VelocityComponent>())
			{
				entity2->get<VelocityComponent>()->velocity = Vector2(0, 0);
			}*/
			bounceEntity(entity1, collisionEvent->collision, entity1PropVector, getVelocity(entity2));
			bounceEntity(entity2, collisionEvent->collision, entity2PropVector, getVelocity(entity1));
		}
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

Vector2 PhysicsSystem::getVelocity(EntityW::EntitySp entity) {
	if (entity->has<VelocityComponent>())
	{
		return entity->get<VelocityComponent>()->velocity;
	}
	return Vector2(0, 0);
}

void PhysicsSystem::bounceEntity(EntitySp entity, Collision& collision, Vector2 prop, Vector2 otherVelocity)
{
	if (entity->has<VelocityComponent>() && entity->has<PhysicsComponent>())
	{
		auto physics = entity->get<PhysicsComponent>();
		auto velocity = entity->get<VelocityComponent>();

		//logger.log("BV " + Vector2Utils::toString(velocity->velocity));
		auto reflected = (velocity->velocity.y != 0) ? glm::reflect(velocity->velocity, collision.normal) : Vector2(-velocity->velocity.x, 0);
		velocity->velocity.x = physics->constraints.x ? reflected.x : velocity->velocity.x;
		velocity->velocity.y = physics->constraints.y ? reflected.y : velocity->velocity.y;
		auto energyVector = Vector2(prop.x * otherVelocity.x, prop.y * otherVelocity.y);

		float velocityLength = glm::length(velocity->velocity);
		velocity->velocity -= energyVector;
		velocity->velocity.y = glm::clamp(velocity->velocity.y, (float)-6., (float)6.);

		if (velocityLength != 0)
			velocity->velocity = glm::normalize(velocity->velocity) * velocityLength;

		//logger.log("B "+std::to_string(entity->id)+" "+Vector2Utils::toString(velocity->velocity)+" "+Vector2Utils::toString(energyVector) + " " + Vector2Utils::toString(reflected) + " " + Vector2Utils::toString(collision.normal));
	}
}

void PhysicsSystem::correctEntity(EntityW::EntitySp entity, Collision& collision, Vector2 prop, float multiplier)
{
	auto transform = entity->get<TransformComponent>();
	auto correction = collision.normal * collision.depth * multiplier;
	correction.x *= prop.x;
	correction.y *= prop.y;
	logger.log("C " + std::to_string(collision.depth) + " Normal: " +Vector2Utils::toString(collision.normal)+" Prop: "+Vector2Utils::toString(prop)+ " Correction: "+Vector2Utils::toString(correction));
	transform->position += correction;
}

PhysicsSystem::PhysicsSystem()
{
	EventDispatcher::get().subscribe<CollisionEvent>(EntityW::EventListenerDelegate(this, &PhysicsSystem::OnCollision));
}

void PhysicsSystem::OnCollision(EntityW::EventSp e)
{
	auto collisionEvent = std::dynamic_pointer_cast<CollisionEvent>(e);
	collisions.push_back(collisionEvent);
}