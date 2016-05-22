#pragma once

#include "EntityW\System.h"
#include "Components.h"
#include "EntityW\Entity.h"
#include "Logger.h"

class MovementSystem : public EntityW::System<TransformComponent, VelocityComponent>
{
protected:
	virtual std::string getName() { return "MovementSystem"; }
public:
	virtual void ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime);
};

