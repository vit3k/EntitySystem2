//
// Created by Pawel Witkowski on 01/05/2023.
//

#ifndef PONG_ENGINE_H
#define PONG_ENGINE_H

#include <string>
#include "ScriptManager.h"
#include "RenderSystem.h"
#include "TextRenderingSystem.h"
#include "SpriteRenderSystem.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include "Configuration.h"

class Engine {
private:
    static std::shared_ptr<Engine> instance;
    std::shared_ptr<IScriptManager> scriptManager;
    Engine() {};
    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<TextRenderingSystem> textRenderingSystem;
    std::shared_ptr<SpriteRenderSystem> spriteRenderSystem;
    std::shared_ptr<CollisionSystem> collisionSystem;
    std::shared_ptr<PhysicsSystem> physicsSystem;
    Logger log = Logger::get("Engine");
    bool initialized = false;
public:
    static std::shared_ptr<Engine> getInstance();
    void run();
    void start(std::shared_ptr<IScriptManager> scriptManager);
    void init(Configuration config);
};


#endif //PONG_ENGINE_H
