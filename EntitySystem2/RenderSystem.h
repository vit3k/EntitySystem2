#pragma once
#include "Components.h"
#include "EntityW\System.h"

class RenderSystem : public EntityW::System<RenderComponent, TransformComponent> {
	sf::RenderWindow* renderWindow;
public:
	void virtual Process(EntityW::Time deltaTime);
	RenderSystem(sf::RenderWindow* renderWindow) : renderWindow(renderWindow) {};
};