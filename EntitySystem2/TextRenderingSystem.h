#pragma once
#include "EntityW/System.h"
#include "Components.h"

class TextRenderingSystem : public EntityW::System<TransformComponent, TextComponent>
{
	sf::RenderTarget* renderTarget;
	sf::Font font;
protected:
	virtual std::string getName() { return "TextRenderingSystem"; }
public:
	TextRenderingSystem(sf::RenderTarget* renderTarget) : renderTarget(renderTarget) {
		font.loadFromFile("cour.ttf");

	};
	virtual void ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime);
};

