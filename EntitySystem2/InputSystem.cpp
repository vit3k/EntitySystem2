#include "InputSystem.h"
#include "EventDispatcher.h"

InputSystem::InputSystem()
{
	EventDispatcher::get().subscribe<MoveUpEvent>(EventListenerDelegate(this, &InputSystem::OnMoveUp));
	EventDispatcher::get().subscribe<MoveDownEvent>(EventListenerDelegate(this, &InputSystem::OnMoveDown));
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

	(*entityState).clear();
}

void InputSystem::OnMoveUp(EventSp e)
{
	auto iE = std::dynamic_pointer_cast<MoveUpEvent>(e);
	state[iE->controller][Up] = true;
}
void InputSystem::OnMoveDown(EventSp e)
{
	auto iE = std::dynamic_pointer_cast<MoveDownEvent>(e);
	state[iE->controller][Down] = true;
}