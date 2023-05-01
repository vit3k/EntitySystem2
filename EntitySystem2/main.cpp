#include "engine/Engine.h"

int main()
{
    auto engine = Engine::getInstance();
    engine->start("scripts/sprite.lua");
	return 0;
}
