#include "InputController.h"
#include <SFML\Window.hpp>

#include "EventDispatcher.h"
#include "Event.h"

void InputController::process()
{
	sf::Keyboard keyboard;
	if (keyboard.isKeyPressed(sf::Keyboard::W))
	{
		EventDispatcher::get().emit<MoveUpEvent>(0);
	}
	else if (keyboard.isKeyPressed(sf::Keyboard::S)) 
	{
		EventDispatcher::get().emit<MoveDownEvent>(0);
	}

	if (keyboard.isKeyPressed(sf::Keyboard::Up))
	{
		EventDispatcher::get().emit<MoveUpEvent>(1);
	}
	else if (keyboard.isKeyPressed(sf::Keyboard::Down))
	{
		EventDispatcher::get().emit<MoveDownEvent>(1);
	}
}