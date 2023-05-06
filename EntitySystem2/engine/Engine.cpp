//
// Created by Pawel Witkowski on 01/05/2023.
//

#include <SFML/Graphics.hpp>
#include "Engine.h"
#include <iostream>
#include "RenderSystem.h"
#include "TextRenderingSystem.h"
#include "InputController.h"
#include "EntityW/EventDispatcher.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include <algorithm>
#include "ScriptManager.h"
#include "SpriteRenderSystem.h"

std::shared_ptr<Engine> Engine::instance;

void Engine::start(std::shared_ptr<IScriptManager> scriptManager) {
    this->scriptManager = scriptManager;
    scriptManager->init();
    if (!initialized) {
        init(Configuration());
    }
    log.log("Engine started...");
    run();
}

void Engine::run()
{
    auto fpsText = EntityW::Entity::create();
    fpsText->attach<TransformComponent>(Vector2(-9.5, -7));
    fpsText->attach<TextComponent>("0");
    fpsText->commit();

    // Main loop
    sf::Clock timer;
    while (window->isOpen())
    {
        sf::Event event{};
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                std::cout << "Window closed" << std::endl;
                window->close();
            }
        }
        EntityW::Time delta(timer.restart().asMicroseconds());
        auto fps = 1. / delta.asSeconds();
        fpsText->get<TextComponent>()->text = std::to_string(fps);

        //event bus before scripts
        EntityW::EventDispatcher::get().process();

        // systems from lua
        //scriptManager->process(delta);

        // event bus after scripts
        EntityW::EventDispatcher::get().process();

        //attachSystem->Process(delta);
        collisionSystem->Process(delta);
        physicsSystem->Process(delta);

        //rendering pipeline
        window->clear();
        renderSystem->Process(delta);
        spriteRenderSystem->Process(delta);
        textRenderingSystem->Process(delta);
        window->display();
    }
    //scriptManager->close();
    std::cout << "Exit" << std::endl;
}

std::shared_ptr<Engine> Engine::getInstance() {
    if (!instance) {
        instance = std::shared_ptr<Engine>(new Engine());
    }
    return instance;
}

void Engine::init(Configuration config) {
    window = std::make_shared<sf::RenderWindow>(sf::VideoMode(config.window.width, config.window.height),
                            config.window.title);
    window->setFramerateLimit(config.frameLimit);

    // Base systems
    renderSystem = std::make_shared<RenderSystem>(std::dynamic_pointer_cast<sf::RenderTarget>(window));
    textRenderingSystem = std::make_shared<TextRenderingSystem>(std::dynamic_pointer_cast<sf::RenderTarget>(window));
    spriteRenderSystem = std::make_shared<SpriteRenderSystem>(std::dynamic_pointer_cast<sf::RenderTarget>(window));
    collisionSystem = std::make_shared<CollisionSystem>();
    physicsSystem = std::make_shared<PhysicsSystem>();

    EntityW::EventDispatcher::get().emitNow<StartedEvent>();
    initialized = true;
}
