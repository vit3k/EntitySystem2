#include "engine/Engine.h"
#include "engine/DotnetScriptManager.h"

int main()
{
    auto engine = Engine::getInstance();
    //auto scriptManager = std::make_shared<ScriptManager>("scripts/sprite.lua");
    
    auto scriptManager = std::make_shared<DotnetScriptManager>("Sprite.Game", "Init");
    engine->start(scriptManager);

	return 0;
}
