#pragma once

#include "EntityW\Entity.h"
#include "Event.h"
#include "EntityW\EventDispatcher.h"
#include "CollisionSystem.h"

class ScoreManager {
	EntityW::EntitySp ball;
	EntityW::EntitySp scoreText;
	int score[2] = { 0, 0 };
public:
	ScoreManager(EntityW::EntitySp ball, EntityW::EntitySp scoreText)
		: ball(ball), scoreText(scoreText) {
		EntityW::EventDispatcher::get().subscribe<CollisionEvent>(EntityW::EventListenerDelegate(this, &ScoreManager::OnCollision));
	}

	void OnCollision(EntityW::EventSp e);
};
