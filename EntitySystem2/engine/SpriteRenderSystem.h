#pragma once
#include "Components.h"
#include "Animation.h"
#include "EntityW/System.h"

class SpriteRenderSystem : public EntityW::System<SpriteComponent, TransformComponent> {
	std::shared_ptr<sf::RenderTarget> renderTarget;
protected:
	virtual std::string getName() { return "SpriteRenderSystem"; }
public:
	void virtual Process(EntityW::Time deltaTime);
	SpriteRenderSystem(std::shared_ptr<sf::RenderTarget> renderTarget) : renderTarget(renderTarget) {};
};