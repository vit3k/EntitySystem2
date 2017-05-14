#include <iostream>
#include <bitset>

#include "EntityW/Entity.h"
#include "Components.h"
#include "RenderSystem.h"
#include "TextRenderingSystem.h"
#include "InputController.h"
#include "InputSystem.h"
#include "EntityW\EventDispatcher.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include "AttachSystem.h"
#include "sol.hpp"
#include <algorithm>
#include "EntityW\ClassTypeId.h"
#include "ScriptManager.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");

	std::shared_ptr<RenderSystem> renderSystem(new RenderSystem(&window));
	std::shared_ptr<TextRenderingSystem> textRenderingSystem(new TextRenderingSystem(&window));
	//auto movementSystem = std::make_shared<MovementSystem>();
	auto inputSystem = std::make_shared<InputSystem>();
	auto collisionSystem = std::make_shared<CollisionSystem>();
	auto physicsSystem = std::make_shared<PhysicsSystem>();
	auto attachSystem = std::make_shared<AttachSystem>();

	sf::Clock timer;
	
	sf::Time lastMillis = timer.getElapsedTime();
	InputController inputController(0);

	ScriptManager scriptManager;
	scriptManager.init();
	scriptManager.run("pong.lua");
	sf::Time lastScriptUpdate = timer.getElapsedTime();
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		sf::Time currentMillis = timer.getElapsedTime();
		EntityW::Time delta((currentMillis - lastMillis).asMicroseconds());
		lastMillis = currentMillis;

		// low level systems
		inputController.process();

		//event bus before scripts
		EntityW::EventDispatcher::get().process();

		// logic systems
		inputSystem->Process(delta);
		// script could be updated less often but somehow this is not working with collision and physics right now
		//if ((currentMillis - lastScriptUpdate).asMilliseconds() > (1000 / 100))
		//{
			//EntityW::Time scriptDelta((currentMillis - lastScriptUpdate).asMicroseconds());
			//scriptManager.process(scriptDelta);
			//lastScriptUpdate = currentMillis;
		//}
		// systems from lua
		scriptManager.process(delta);

		// event bus after scripts
		EntityW::EventDispatcher::get().process();

		//attachSystem->Process(delta);
		collisionSystem->Process(delta);
		physicsSystem->Process(delta);

		//rendering pipeline
		window.clear();
		renderSystem->Process(delta);
		textRenderingSystem->Process(delta);
		window.display();
		
	}
	
	return 0;
}


