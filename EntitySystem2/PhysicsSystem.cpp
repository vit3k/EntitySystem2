#include "PhysicsSystem.h"
#include "EventDispatcher.h"
#include "EntityW\Entity.h"
#include <sstream>

using namespace EntityW;

void PhysicsSystem::Process(EntityW::Time deltaTime)
{
	for (auto collisionEvent : collisions)
	{
		auto entity1 = collisionEvent->entity1;
		auto entity2 = collisionEvent->entity2;
		
		float entity1Mass = getMass(entity1);
		float entity2Mass = getMass(entity2);
		float entity1Prop = entity1Mass / (entity1Mass + entity2Mass);
		float entity2Prop = -(entity2Mass / (entity1Mass + entity2Mass));
		
		correctEntity(entity1, collisionEvent->collision, entity1Prop);
		correctEntity(entity2, collisionEvent->collision, entity1Prop);

	}
	collisions.clear();
}

float PhysicsSystem::getMass(EntityW::EntitySp entity)
{
	if (entity->has<PhysicsComponent>())
	{
		return entity->get<PhysicsComponent>()->mass;
	}
	return 0.;
}

void PhysicsSystem::bounceEntity(EntitySp entity)
{
	if (entity->has<VelocityComponent>() && entity->has<PhysicsComponent>())
	{

	}
}

void PhysicsSystem::correctEntity(EntityW::EntitySp entity, Collision& collision, float prop)
{
	auto transform = entity->get<TransformComponent>();
	auto correction = collision.normal * collision.depth * prop;
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