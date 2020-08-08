#include "RenderSystem.h"

void RenderSystem::Process(EntityW::Time deltaTime)
{
	for (auto pair : entities) {
		auto entity = pair.second;
		auto render = entity->get<RenderComponent>();
		auto transform = entity->get<TransformComponent>();
		auto shape = render->shape;

		auto projectedPosition = Vector2((transform->position.x + 10) * 40, (transform->position.y + 7.5) * 40);
		shape->setPosition(projectedPosition.x, projectedPosition.y);
		shape->setScale(40, 40);
		renderWindow->draw(*shape);
	}
}