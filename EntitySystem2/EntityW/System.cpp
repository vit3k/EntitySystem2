#include "System.h"
#include "../Components.h"

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

		if (entity->hasComponents(components)) {
			logger.log(getName() + ": entity added");
			entities[entity->id] = entity;
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
		if (script["process"].valid())
		{
			script["process"](deltaTime);
		}
		else if (script["processEntity"].valid())
		{
			for (auto entity : entities)
			{
				script["processEntity"](script, entity.second, deltaTime);
			}
		}
	}

	void ScriptSystem::OnScriptComponentAttached(EventSp event)
	{
		auto componentAttachedEvent = std::dynamic_pointer_cast<ScriptComponentAttachedEvent>(event);
		auto entity = componentAttachedEvent->entity;

		if (entity->hasComponents(components)) {
			logger.log(getName() + ": entity added");
			entities[entity->id] = entity;
		}
	}

	void ScriptSystem::OnScriptComponentDetached(EventSp event)
	{
		auto componentDetachedEvent = std::dynamic_pointer_cast<ScriptComponentDetachedEvent>(event);
		auto entity = componentDetachedEvent->entity;

		if (!entity->hasComponents(components)) {
			logger.log(getName() + ": entity removed");
			entities[entity->id] = entity;
			entities.erase(entity->id);
		}
	}
}
