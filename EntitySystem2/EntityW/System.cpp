#include "System.h"

namespace EntityW {
	void BaseSystem::ProcessEntity(EntitySp entity, Time deltaTime)
	{

	}

	void BaseSystem::OnEntityAdded(EntitySp entity)
	{
		entities.push_back(entity);
	}

	void BaseSystem::Process(Time deltaTime)
	{
		for (auto entity : entities)
		{
			ProcessEntity(entity, deltaTime);
		}
	}
	
}
