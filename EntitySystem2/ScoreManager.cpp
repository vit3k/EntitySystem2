#include "ScoreManager.h"

void ScoreManager::OnCollision(EntityW::EventSp e) {
	auto collisionEvent = std::dynamic_pointer_cast<CollisionEvent>(e);
	
	EntityW::EntitySp scoringSurface = nullptr;

	if (collisionEvent->entity1->has<ScoringSurfaceComponent>()) {
		scoringSurface = collisionEvent->entity1;
	}
	else if (collisionEvent->entity2->has<ScoringSurfaceComponent>()) {
		scoringSurface = collisionEvent->entity2;
	}

	if (scoringSurface != nullptr) {
		auto scoring = scoringSurface->get<ScoringSurfaceComponent>();
		
		score[scoring->playerId]++;
		scoreText->get<TextComponent>()->text = std::to_string(score[0]) + " - " + std::to_string(score[1]);
		auto paddleTransform = scoring->paddle->get<TransformComponent>();
		Vector2 relativePosition = Vector2(glm::normalize(paddleTransform->position).x * (-1), .75);
		ball->attach<AttachComponent>(paddleTransform, relativePosition);
	}
}