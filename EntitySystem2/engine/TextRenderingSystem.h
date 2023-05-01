#pragma once
#include "EntityW/System.h"
#include "Components.h"

class TextRenderingSystem : public EntityW::System<TransformComponent, TextComponent>
{
	std::shared_ptr<sf::RenderTarget> renderTarget;
	sf::Font font;
protected:
	virtual std::string getName() { return "TextRenderingSystem"; }
public:
	TextRenderingSystem(std::shared_ptr<sf::RenderTarget>  renderTarget) : renderTarget(renderTarget) {
		font.loadFromFile("assets/cour.ttf");

	};
	virtual void ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime);
};

