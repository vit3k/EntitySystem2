#pragma once
#include "EntityW/System.h"
#include "Components.h"
#include "Collider.h"
#include "Event.h"
#include "Logger.h"
#include "EntityW/Entity.h"
#include <memory>

typedef std::pair<ShapeType, ShapeType> ColliderType;

class CollisionSystem : public EntityW::System<TransformComponent, CollisionComponent>
{
	std::map<ColliderType, std::shared_ptr<Collider>> colliders;
protected:
	virtual std::string getName() { return "CollisionSystem"; }
public:
	virtual void Process(EntityW::Time delta);
	CollisionSystem();
	~CollisionSystem();
};

class CollisionEvent : public EntityW::Event<CollisionEvent>
{
public:
	Collision collision;
	EntityW::EntitySp entity1;
	EntityW::EntitySp entity2;
	CollisionEvent(Collision collision, EntityW::EntitySp entity1, EntityW::EntitySp entity2) : collision(collision), entity1(entity1), entity2(entity2) {

	}
};

typedef std::shared_ptr<CollisionEvent> CollisionEventSp;