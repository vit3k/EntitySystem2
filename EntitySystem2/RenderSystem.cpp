#include "RenderSystem.h"

void RenderSystem::Process(EntityW::Time deltaTime)
{
	for (EntityW::EntitySp entity : entities) {
		auto render = entity->get<RenderComponent>();
		auto transform = entity->get<TransformComponent>();
		auto shape = render->shape;
		auto windowSize = renderWindow->getSize();

		sf::View view;
		view.reset(sf::FloatRect(-10., -7.5, 20., 15. ));
		renderWindow->setView(view);

		shape->setPosition(transform->position.x, transform->position.y);
		renderWindow->draw(*shape);
	}	
}