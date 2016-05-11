#include "MovementSystem.h"

void MovementSystem::ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime)
{
	auto transform = entity->get<TransformComponent>();
	auto velocity = entity->get<VelocityComponent>();
	
	transform->position += (velocity->velocity * deltaTime.asSeconds());
}