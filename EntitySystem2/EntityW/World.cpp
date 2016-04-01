#include "World.h"

namespace EntityW {
	EntitySp World::createEntity() {
		EntitySp entity = std::make_shared<Entity>(this, getNextEntityId());
		entities.push_back(entity);
		added.push_back(entity);
		return entity;
	};

	void World::attachSystem(SystemSp system)
	{
		systems.push_back(system);
		system->setWorld(this);
	}

	void World::refresh()
	{
		// simple implementation for now
		for (EntitySp entity : added) 
		{
			for (SystemSp system : systems) {
				if (entity->hasComponents(system->components)) {
					system->OnEntityAdded(entity);
				}
			}
		}
		added.clear();
	}


}