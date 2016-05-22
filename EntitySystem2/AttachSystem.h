#pragma once
#include "EntityW\System.h"
#include "Components.h"

class AttachSystem :
	public EntityW::System<TransformComponent, AttachComponent>
{
protected:
	virtual std::string getName() { return "AttachSystem"; }
public:
	virtual void ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime);
	virtual void OnComponentAttached(EntityW::EventSp event);
};

