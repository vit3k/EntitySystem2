#include "AttachSystem.h"

void AttachSystem::ProcessEntity(EntityW::EntitySp entity, EntityW::Time deltaTime) {
	auto transform = entity->get<TransformComponent>();
	auto attach = entity->get<AttachComponent>();

	transform->position = attach->parentTransform->position + attach->relativePosition;
}

void AttachSystem::OnComponentAttached(EntityW::EventSp event) {
	BaseSystem::OnComponentAttached(event);
}