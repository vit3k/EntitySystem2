#include "Collider.h"
#include "Components.h"
#include <cmath>

Collision BoxCircleCollider::collide(EntityW::EntitySp entity1, EntityW::EntitySp entity2)
{
	auto transform1 = entity1->get<TransformComponent>();
	auto transform2 = entity2->get<TransformComponent>();
	auto collision1 = entity1->get<CollisionComponent>();
	auto collision2 = entity2->get<CollisionComponent>();

	auto collisionShape1 = (RectCollisionShape*) collision1->shape;
	auto collisionShape2 = (CircleCollisionShape*)collision2->shape;

	Vector2 circleCenter = transform2->position + collisionShape2->center();
	Vector2* vertices = calculateVertices(entity1);
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

	return Collision();
}

Collision BoxBoxCollider::collide(EntityW::EntitySp entity1, EntityW::EntitySp entity2)
{
	auto transform1 = entity1->get<TransformComponent>();
	auto transform2 = entity2->get<TransformComponent>();
	auto collision1 = entity1->get<CollisionComponent>();
	auto collision2 = entity2->get<CollisionComponent>();

	auto collisionShape1 = (RectCollisionShape*)collision1->shape;
	auto collisionShape2 = (RectCollisionShape*)collision2->shape;

	Collision collision;
	collision.occured = true;

	Vector2 axes[2] = { Vector2(1., 0.), Vector2(0., 1.)};
	Vector2 smallest;
	float minOverlap = 10000000;
	for (auto axis : axes) {
		Projection projection1 = project(entity1, axis);
		Projection projection2 = project(entity2, axis);
		//logger.log(std::to_string(projection1.min)+" "+std::to_string( projection1.max)+" "+std::to_string(projection2.min)+" "+std::to_string(projection2.max));
		float overlap = projection1.overlaps(projection2);
		//logger.log(std::to_string(overlap));
		if ( overlap == 0)
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

Vector2* Collider::calculateVertices(EntityW::EntitySp entity)
{
	auto transform = entity->get<TransformComponent>();
	auto collision = entity->get<CollisionComponent>();
	auto collisionShape = (RectCollisionShape*)collision->shape;

	Vector2 vertices[4] = { Vector2(transform->position.x, transform->position.y),
		Vector2(transform->position.x + collisionShape->width, transform->position.y),
		Vector2(transform->position.x + collisionShape->width, transform->position.y + collisionShape->height),
		Vector2(transform->position.x, transform->position.y + collisionShape->height) };
	//for (int i = 0; i < 4; i++) {
	logger.log(std::to_string(vertices[0].x) + " " + std::to_string(vertices[0].y));
	//}
	
	return vertices;
}

Projection Collider::project(EntityW::EntitySp entity, Vector2 axis)
{
	Vector2* vertices = calculateVertices(entity);
	logger.log(std::to_string(vertices[0].x));
	//logger.log(std::to_string(vertices[0].x) + " " + std::to_string(vertices[0].y));// +" " + std::to_string(axis.x) + " " + std::to_string(axis.y));
	float max = glm::dot(axis, vertices[0]);
	

	float min = max;
	
	for (int i = 1; i < 4; i++)
	{
		float p = glm::dot(axis, vertices[i]);
		if (p < min)
		{
			min = p;
		}
		else if (p > max)
		{
			max = p;
		}
	}
	
	return Projection(min, max);
}