#include "Collider.h"
#include "Components.h"
#include <cmath>

Collision Collider::sat(EntityW::EntitySp entity1, EntityW::EntitySp entity2, std::vector<Vector2> axes) {
	auto transform1 = entity1->get<TransformComponent>();
	auto transform2 = entity2->get<TransformComponent>();
	auto collision1 = entity1->get<CollisionComponent>();
	auto collision2 = entity2->get<CollisionComponent>();

	auto collisionShape1 = std::static_pointer_cast<RectCollisionShape>(collision1->shape);
	auto collisionShape2 = std::static_pointer_cast<RectCollisionShape>(collision2->shape);

	Collision collision;
	collision.occured = true;

	Vector2 smallest;
	float minOverlap = 10000000;
	for (auto axis : axes) {
		Projection projection1 = collisionShape1->project(transform1, axis);
		Projection projection2 = collisionShape2->project(transform2, axis);
		float overlap = projection1.overlaps(projection2);
		if (overlap == 0)
		{
			collision.occured = false;
			break;
		}
		else
		{
			if (overlap < minOverlap)
			{
				minOverlap = overlap;
				smallest = axis;
			}
		}
	}
	if (collision.occured)
	{
		Vector2 center1 = transform1->position + collisionShape1->center();
		Vector2 center2 = transform2->position + collisionShape2->center();
		Vector2 center12 = center1 - center2;
		if (glm::dot(smallest, center12) < 0)
		{
			smallest = -smallest;
		}
		collision.normal = smallest;
		collision.depth = minOverlap;
	}
	return collision;
}
Collision BoxCircleCollider::collide(EntityW::EntitySp entity1, EntityW::EntitySp entity2)
{
	auto transform1 = entity1->get<TransformComponent>();
	auto transform2 = entity2->get<TransformComponent>();
	auto collision1 = entity1->get<CollisionComponent>();
	auto collision2 = entity2->get<CollisionComponent>();

	auto collisionShape1 = std::static_pointer_cast<RectCollisionShape>( collision1->shape);
	auto collisionShape2 = std::static_pointer_cast<CircleCollisionShape>(collision2->shape);

	Vector2 circleCenter = transform2->position + collisionShape2->center();
	std::vector<Vector2> vertices = collisionShape1->calculateVertices(transform1);
	Vector2 axis;
	float minDistance = 10000000000;
	for (int i = 0; i < 4; i++)
	{
		Vector2 vertex = vertices[i];
		Vector2 distanceVector = circleCenter - vertex;
		float distance = fabs(glm::length(distanceVector));
		if (distance < minDistance)
		{
			minDistance = distance;
			axis = distanceVector;
		}
	}
	axis = glm::normalize(axis);
	
	return sat(entity1, entity2, { Vector2(1., 0.), Vector2(0., 1.), axis });
}

Collision BoxBoxCollider::collide(EntityW::EntitySp entity1, EntityW::EntitySp entity2)
{
	return sat(entity1, entity2, { Vector2(1., 0.), Vector2(0., 1.) });
}
