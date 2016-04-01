#include "Entity.h"

namespace EntityW {
	bool Entity::hasComponents(ComponentList componentList) {
		return (this->componentList & componentList) == componentList;
	};
}