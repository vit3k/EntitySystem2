#include "Entity.h"

namespace EntityW {
	int Entity::currentEntityId = 0;
	std::map<long, std::shared_ptr<Entity>> Entity::entities;

	bool Entity::hasComponents(ComponentList componentList) {
		return (this->componentList & componentList) == componentList;
	};
}