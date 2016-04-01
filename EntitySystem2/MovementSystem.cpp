#include "MovementSystem.h"

void MovementSystem::ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime)
{
	auto transform = entity->get<TransformComponent>();
	auto velocity = entity->get<VelocityComponent>();
	
	//if (glm::length(velocity->velocity) != 0)
		//Slogger.log(Vector2Utils::toString(velocity->velocity * deltaTime.asSeconds()));
	
	transform->position += (velocity->velocity * deltaTime.asSeconds());
}