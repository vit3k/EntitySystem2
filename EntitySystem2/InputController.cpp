#include "InputController.h"
#include <SFML\Window.hpp>

#include "EntityW\EventDispatcher.h"
#include "Event.h"

void InputController::process()
{
	sf::Keyboard keyboard;
	if (keyboard.isKeyPressed(sf::Keyboard::W))
	{
		EntityW::EventDispatcher::get().emit<MoveUpEvent>(0);
	}
	else if (keyboard.isKeyPressed(sf::Keyboard::S)) 
	{
		EntityW::EventDispatcher::get().emit<MoveDownEvent>(0);
	}
	else if (keyboard.isKeyPressed(sf::Keyboard::Space)) 
	{
		EntityW::EventDispatcher::get().emit<LaunchBallEvent>(0);
	}

	if (keyboard.isKeyPressed(sf::Keyboard::Up))
	{
		EntityW::EventDispatcher::get().emit<MoveUpEvent>(1);
	}
	else if (keyboard.isKeyPressed(sf::Keyboard::Down))
	{
		EntityW::EventDispatcher::get().emit<MoveDownEvent>(1);
	}
	else if (keyboard.isKeyPressed(sf::Keyboard::Space)) 
	{
		EntityW::EventDispatcher::get().emit<LaunchBallEvent>(1);
	}
}