#include "CollisionSystem.h"
#include "Collider.h"
#include "EntityW\EventDispatcher.h"

CollisionSystem::CollisionSystem() {
	colliders[ColliderType(Rect, Rect)] = new BoxBoxCollider();
	colliders[ColliderType(Rect, Circle)] = new BoxCircleCollider();
}

CollisionSystem::~CollisionSystem() {
	for (auto elem : colliders)
	{
		delete elem.second;
	}
	colliders.clear();
}

void CollisionSystem::Process(EntityW::Time delta)
{
	for (int i = 0; i < entities.size(); i++)
	{
		auto entity1 = entities[i];
		auto collisionComponent1 = entity1->get<CollisionComponent>();
		for (int j = i + 1; j < entities.size(); j++)
		{
			auto entity2 = entities[j];
			auto collisionComponent2 = entity2->get<CollisionComponent>();

			Collider* collider = colliders[ColliderType(collisionComponent1->shape->getType(), collisionComponent2->shape->getType())];

			Collision collision;

			if (collider != NULL) 
			{
				collision = collider->collide(entity1, entity2);
			}
			else 
			{
				collider = colliders[std::pair<CollisionShapeType, CollisionShapeType>(collisionComponent2->shape->getType(), collisionComponent1->shape->getType())];

				if (collider != NULL) {
					collision = collider->collide(entity2, entity1);
				}
				else {
					logger.log("No matching collider");
				}
			}
			
			if (collision.occured)
			{
				//logger.log("Collision!");
				EntityW::EventDispatcher::get().emitNow<CollisionEvent>(collision, entity1, entity2);
			}
		}
	}
}
