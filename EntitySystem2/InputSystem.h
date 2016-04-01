#pragma once

#include "EntityW\System.h"
#include "Components.h"
#include "Event.h"
#include "EntityW\Time.h"

enum Move {
	Up, Down
};
class InputSystem : public EntityW::System<VelocityComponent, InputComponent>
{
	std::map<int, std::map<Move, bool>> state;
public:
	InputSystem();
	virtual void ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime);
	void OnMoveUp(EventSp e);
	void OnMoveDown(EventSp e);
};
