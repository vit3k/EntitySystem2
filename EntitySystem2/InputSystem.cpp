#include "InputSystem.h"
#include "EntityW\EventDispatcher.h"

InputSystem::InputSystem()
{
	EntityW::EventDispatcher::get().subscribe<MoveUpEvent>(EntityW::EventListenerDelegate(this, &InputSystem::OnMoveUp));
	EntityW::EventDispatcher::get().subscribe<MoveDownEvent>(EntityW::EventListenerDelegate(this, &InputSystem::OnMoveDown));
}
void InputSystem::ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime)
{
	auto input = entity->get<InputComponent>();
	auto velocity = entity->get<VelocityComponent>();

	auto entityState = &state[input->controller];
	if ((*entityState)[Up])
		velocity->velocity.y = -10;
	else if ((*entityState)[Down])
		velocity->velocity.y = 10;
	else
		velocity->velocity.y = 0;

	if ((*entityState)[LaunchBall]) 
	{
		
	}
	
	(*entityState).clear();
}

void InputSystem::OnMoveUp(EntityW::EventSp e)
{
	auto iE = std::dynamic_pointer_cast<MoveUpEvent>(e);
	state[iE->controller][Up] = true;
}
void InputSystem::OnMoveDown(EntityW::EventSp e)
{
	auto iE = std::dynamic_pointer_cast<MoveDownEvent>(e);
	state[iE->controller][Down] = true;
}

void InputSystem::OnLaunchBall(EntityW::EventSp e)
{
	auto iE = std::dynamic_pointer_cast<LaunchBallEvent>(e);
	state[iE->controller][LaunchBall] = true;
}