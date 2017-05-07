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
	for (auto entity1i = entities.begin(); entity1i != entities.end(); entity1i++)
	{
		auto entity1 = entity1i->second;
		auto collisionComponent1 = entity1->get<CollisionComponent>();
		for (auto entity2i = entity1i; entity2i != entities.end(); entity2i++)
		{
			auto entity2 = entity2i->second;
			if (entity1 == entity2)
			{
				continue;
			}
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
