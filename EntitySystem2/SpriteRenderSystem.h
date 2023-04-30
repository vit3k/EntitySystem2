#pragma once
#include "Components.h"
#include "Animation.h"
#include "EntityW/System.h"

class SpriteRenderSystem : public EntityW::System<SpriteComponent, TransformComponent> {
	sf::RenderWindow* renderWindow;
protected:
	virtual std::string getName() { return "SpriteRenderSystem"; }
public:
	void virtual Process(EntityW::Time deltaTime);
	SpriteRenderSystem(sf::RenderWindow* renderWindow) : renderWindow(renderWindow) {};
};