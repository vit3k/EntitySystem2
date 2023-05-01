#include "SpriteRenderSystem.h"

void SpriteRenderSystem::Process(EntityW::Time deltaTime)
{
	for (auto pair : entities) {
		auto entity = pair.second;
		auto sprite = entity->get<SpriteComponent>();
		auto transform = entity->get<TransformComponent>();
		auto projectedPosition = Vector2((transform->position.x + 10) * 40, (transform->position.y + 7.5) * 40);
		sprite->sprite->setPosition(projectedPosition.x, projectedPosition.y);
        sprite->sprite->setScale(sprite->scale.x, sprite->scale.y);
        renderTarget->draw(*(sprite->sprite));
	}
}