#ifndef WORLD_H
#define WORLD_H

#include "Entity.h"
#include <map>
#include "System.h"

namespace EntityW {
	class World {
		std::vector<EntitySp> entities;
		std::vector<SystemSp> systems;
		std::vector<EntitySp> added;
		
		int currentEntityId = 0;
		int currentSystemId = 0;
		int getNextEntityId() { return currentEntityId++; }
		int getNextSystemId() { return currentSystemId++; }
	public:
		EntitySp createEntity();
		void attachSystem(SystemSp system);
		void refresh();
	};

	typedef std::shared_ptr<World> WorldSp;
}

#endif