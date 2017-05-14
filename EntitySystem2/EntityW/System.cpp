#include "System.h"

namespace EntityW {
	void BaseSystem::ProcessEntity(EntitySp entity, Time deltaTime)
	{

	}

	void BaseSystem::OnEntityAdded(EventSp event)
	{
		auto entityCreatedEvent = std::dynamic_pointer_cast<EntityCreatedEvent>(event);
		
		auto entity = entityCreatedEvent->entity;
		if (entity->hasComponents(components)) {
			logger.log(getName() + ": entity added");
			entities[entity->id] = entity;
		}
	}

	void BaseSystem::OnComponentAttached(EventSp event) {
		auto entityCreatedEvent = std::dynamic_pointer_cast<ComponentAttachedEvent>(event);
		auto entity = entityCreatedEvent->entity;
		if (entities.find(entity->id) == entities.end()) {
			if (entity->hasComponents(components)) {
				logger.log(getName()+": entity added");
				entities[entity->id] = entity;
			}
		}
	}

	void BaseSystem::Process(Time deltaTime)
	{
		for (auto entity : entities)
		{
			ProcessEntity(entity.second, deltaTime);
		}
	}
	
	void ScriptSystem::Process(EntityW::Time deltaTime)
	{
		if (script["process"] != nullptr)
		{
			script["process"](deltaTime);
		}
		else
		{
			for (auto entity : entities)
			{
				script["processEntity"](entity, deltaTime);
			}
		}
	}
}
