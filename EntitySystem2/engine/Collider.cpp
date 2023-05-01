#include "Collider.h"
#include "Components.h"
#include <cmath>

Collision Collider::sat(EntityW::EntitySp entity1, EntityW::EntitySp entity2, std::vector<Vector2> axes) {
	auto transform1 = entity1->get<TransformComponent>();
	auto transform2 = entity2->get<TransformComponent>();
	auto collision1 = entity1->get<CollisionComponent>();
	auto collision2 = entity2->get<CollisionComponent>();

	Collision collision;
	collision.occured = true;

	Vector2 smallest;
	float minOverlap = 10000000;
	for (auto axis : axes) {
		Projection projection1 = collision1->shape->project(transform1, axis);
		Projection projection2 = collision2->shape->project(transform2, axis);
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
		Vector2 center1 = transform1->position + collision1->shape->center();
		Vector2 center2 = transform2->position + collision2->shape->center();
		Vector2 center12 = center2 - center1;
		logger.log("Collider before: " + Vector2Utils::toString(smallest));
		if (glm::dot(smallest, center12) > 0)
		{
			smallest = -smallest;
		}
		collision.normal = smallest;
		collision.depth = minOverlap;
		logger.log("Collider after: " + Vector2Utils::toString(smallest));
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
