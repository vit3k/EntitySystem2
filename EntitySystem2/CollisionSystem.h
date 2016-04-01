#pragma once
#include "EntityW\System.h"
#include "Components.h"
#include "Collider.h"
#include "Event.h"
#include "Logger.h"
#include "EntityW\Entity.h"

typedef std::pair<CollisionShapeType, CollisionShapeType> ColliderType;

class CollisionSystem : public EntityW::System<TransformComponent, CollisionComponent>
{
	std::map<ColliderType, Collider*> colliders;
	Logger logger = Logger::get("ColliionSystem");
public:
	virtual void Process(EntityW::Time delta);
	CollisionSystem();
	~CollisionSystem();
};

class CollisionEvent : public Event<CollisionEvent>
{
public:
	Collision collision;
	EntityW::EntitySp entity1;
	EntityW::EntitySp entity2;
	CollisionEvent(Collision collision, EntityW::EntitySp entity1, EntityW::EntitySp entity2) : collision(collision), entity1(entity1), entity2(entity2) {}
};

typedef std::shared_ptr<CollisionEvent> CollisionEventSp;