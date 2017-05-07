#include "TextRenderingSystem.h"

void TextRenderingSystem::ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime)
{
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(12);
	text.setString(entity->get<TextComponent>()->text);
	text.setFillColor(sf::Color::White);
	auto transform = entity->get<TransformComponent>();
	auto projectedPosition = Vector2((transform->position.x + 10) * 40, (transform->position.y + 7.5) * 40);
	text.setPosition(projectedPosition.x, projectedPosition.y);

	text.setPosition(projectedPosition.x, projectedPosition.y);
	renderTarget->draw(text);
}
