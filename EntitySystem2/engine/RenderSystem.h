#pragma once
#include "Components.h"
#include "EntityW/System.h"

class RenderSystem : public EntityW::System<RenderComponent, TransformComponent> {
	std::shared_ptr<sf::RenderTarget> renderTarget;
protected:
	virtual std::string getName() { return "RenderSystem"; }
public:
	void virtual Process(EntityW::Time deltaTime);
	RenderSystem(std::shared_ptr<sf::RenderTarget> renderTarget) : renderTarget(renderTarget) {};
};