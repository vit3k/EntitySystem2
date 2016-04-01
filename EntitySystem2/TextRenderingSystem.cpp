#include "TextRenderingSystem.h"

void TextRenderingSystem::ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime)
{
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(12);
	text.setString(entity->get<TextComponent>()->text);
	text.setColor(sf::Color::White);
	text.setPosition(entity->get<TransformComponent>()->position.x, entity->get<TransformComponent>()->position.y);
	renderTarget->draw(text);
}
