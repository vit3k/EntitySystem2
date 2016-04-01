#pragma once

#include "EntityW\System.h"
#include "Components.h"
#include "EntityW\Entity.h"
#include "Logger.h"

class MovementSystem : public EntityW::System<TransformComponent, VelocityComponent>
{
	Logger logger = Logger::get("MS");
public:
	virtual void ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime);
};

