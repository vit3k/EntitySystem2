#pragma once

#include "EntityW\System.h"
#include "Components.h"
#include "Event.h"
#include "EntityW\Time.h"

enum Move {
	Up, Down, LaunchBall
};
class InputSystem : public EntityW::System<VelocityComponent, InputComponent>
{
	std::map<int, std::map<Move, bool>> state;
protected:
	virtual std::string getName() { return "InputSystem"; }
public:
	InputSystem();
	virtual void ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime);
	void OnMoveUp(EntityW::EventSp e);
	void OnMoveDown(EntityW::EventSp e);
	void OnLaunchBall(EntityW::EventSp e);
};
