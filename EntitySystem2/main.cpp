#include <iostream>
#include <bitset>

#include "EntityW/Entity.h"
#include "Components.h"
#include "RenderSystem.h"
#include "TextRenderingSystem.h"
#include "InputController.h"
#include "EntityW/EventDispatcher.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include <sol/sol.hpp>
#include <algorithm>
#include "EntityW/ClassTypeId.h"
#include "ScriptManager.h"
#include "Animation.h"
#include "SpriteRenderSystem.h"

int main()
{
	auto vm = sf::VideoMode::getDesktopMode();
	std::cout << vm.width << " x " << vm.height << std::endl;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Sprite");
	//sf::View view(sf::FloatRect(-10f, ))
	window.setFramerateLimit(60);
	std::shared_ptr<RenderSystem> renderSystem(new RenderSystem(&window));
	std::shared_ptr<TextRenderingSystem> textRenderingSystem(new TextRenderingSystem(&window));
	auto collisionSystem = std::make_shared<CollisionSystem>();
	auto physicsSystem = std::make_shared<PhysicsSystem>();
    auto spriteRenderSystem = std::make_shared<SpriteRenderSystem>(&window);
	InputController inputController(0);

	ScriptManager scriptManager;
	scriptManager.init();
	scriptManager.run("scripts/sprite.lua");
	auto fpsText = EntityW::Entity::create();
	fpsText->attach<TransformComponent>(Vector2(-9.5, -6));
	fpsText->attach<TextComponent>("0");
	fpsText->commit();

	EntityW::EventDispatcher::get().emitNow<StartedEvent>();

	sf::Clock timer;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				std::cout << "Window closed" << std::endl;
				window.close();
			}
		}
		EntityW::Time delta(timer.restart().asMicroseconds());
		auto fps = 1. / delta.asSeconds();
		fpsText->get<TextComponent>()->text = std::to_string(fps);
		// low level systems
		inputController.process();

		//event bus before scripts
		EntityW::EventDispatcher::get().process();

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
        spriteRenderSystem->Process(delta);
		textRenderingSystem->Process(delta);
		window.display();
		

	}
	scriptManager.close();
	std::cout << "Exit" << std::endl;
	return 0;
}
