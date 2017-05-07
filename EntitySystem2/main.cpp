#include <iostream>
#include <bitset>

#include "EntityW/Entity.h"
#include "Components.h"
#include "RenderSystem.h"
#include "TextRenderingSystem.h"
#include "InputController.h"
#include "MovementSystem.h"
#include "InputSystem.h"
#include "EntityW\EventDispatcher.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include "ScoreManager.h"
#include "AttachSystem.h"
#include "sol.hpp"

class TestClass {
public:
	virtual int test() { return 1; }
};
/*RenderComponent test(){
	State state;
	state["Vector2"].SetClass<Vector2, double, double>("x", &Vector2::x, "y", &Vector2::y);
	state["TransformComponent"].SetClass<TransformComponent, Vector2>();
	state["InputComponent"].SetClass<InputComponent, int>();
	state["VelocityComponent"].SetClass<VelocityComponent, Vector2, float>();
	state["Shape"].SetClass<sf::Shape>();
	//state["RectangleShape"].SetClass<sf::RectangleShape, Vector2>();
	state["RenderComponent"].SetClass<RenderComponent, sf::Shape>();
	state.Load("test.lua");
	auto entity = state["entity"];
	RenderComponent test = entity["render"];
	return test;
}*/
int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");
	
	std::shared_ptr<RenderSystem> renderSystem(new RenderSystem(&window));
	std::shared_ptr<TextRenderingSystem> textRenderingSystem(new TextRenderingSystem(&window));
	auto movementSystem = std::make_shared<MovementSystem>();
	auto inputSystem = std::make_shared<InputSystem>();
	auto collisionSystem = std::make_shared<CollisionSystem>();
	auto physicsSystem = std::make_shared<PhysicsSystem>();
	auto attachSystem = std::make_shared<AttachSystem>();

	EntityW::EntitySp paddle1 = EntityW::Entity::create();//world.createEntity();
	paddle1->attach<TransformComponent>(Vector2(-10., -1.));
	sf::RectangleShape paddle1Shape(sf::Vector2f(.5, 2.));
	paddle1Shape.setFillColor(sf::Color::Green);
	paddle1->attach<RenderComponent>(&paddle1Shape);
	paddle1->attach<InputComponent>(0);
	paddle1->attach<VelocityComponent>(Vector2(0, 0), 0);
	paddle1->attach<CollisionComponent>(&RectCollisionShape(.5, 2.));
	paddle1->attach<PhysicsComponent>(0., 1., Vector2(0, 1));
	paddle1->commit();

	EntityW::EntitySp paddle2 = EntityW::Entity::create();//world.createEntity();
	paddle2->attach<TransformComponent>(Vector2(9.5, -1.));
	sf::RectangleShape paddle2Shape(sf::Vector2f(.5, 2.));
	paddle2Shape.setFillColor(sf::Color::Red);
	paddle2->attach<RenderComponent>(&paddle2Shape);
	paddle2->attach<InputComponent>(1);
	paddle2->attach<VelocityComponent>(Vector2(0, 0), 0);
	paddle2->attach<CollisionComponent>(&RectCollisionShape(.5, 2.));
	paddle2->attach<PhysicsComponent>(0., 1., Vector2(0, 1));
	paddle2->commit();

	EntityW::EntitySp ball = EntityW::Entity::create();//world.createEntity("ball");
	ball->attach<TransformComponent>(Vector2(-0.15, -5));
	sf::CircleShape ballShape(0.3);
	ballShape.setFillColor(sf::Color::White);
	ball->attach<RenderComponent>(&ballShape);
	ball->attach<VelocityComponent>(Vector2(-8., 4.), 1);
	ball->attach<CollisionComponent>(&CircleCollisionShape(.3));
	ball->attach<PhysicsComponent>(1., .1, Vector2(1, 1));
	ball->commit();

	EntityW::EntitySp topBorder = EntityW::Entity::create();//world.createEntity();
	topBorder->attach<TransformComponent>(Vector2(-10.0, -8.5));
	topBorder->attach<CollisionComponent>(&RectCollisionShape(20.0, 1));
	topBorder->attach<PhysicsComponent>(0., 0., Vector2(0, 0));
	topBorder->commit();

	EntityW::EntitySp bottomBorder = EntityW::Entity::create();//world.createEntity();
	bottomBorder->attach<TransformComponent>(Vector2(-10.0, 7.5));
	bottomBorder->attach<CollisionComponent>(&RectCollisionShape(20.0, 1));
	bottomBorder->attach<PhysicsComponent>(0., 0., Vector2(0, 0));
	bottomBorder->commit();

	EntityW::EntitySp paddle1Border = EntityW::Entity::create();//world.createEntity();
	paddle1Border->attach<TransformComponent>(Vector2(-11.0, -7.5));
	paddle1Border->attach<CollisionComponent>(&RectCollisionShape(1.0, 15.));
	paddle1Border->attach<ScoringSurfaceComponent>(1, paddle1);
	paddle1Border->commit();

	EntityW::EntitySp paddle2Border = EntityW::Entity::create();//world.createEntity();
	paddle2Border->attach<TransformComponent>(Vector2(10.0, -7.5));
	paddle2Border->attach<CollisionComponent>(&RectCollisionShape(1.0, 15.));
	paddle2Border->attach<ScoringSurfaceComponent>(0, paddle2);
	paddle2Border->commit();

	EntityW::EntitySp score = EntityW::Entity::create();//world.createEntity();
	score->attach<TransformComponent>(Vector2(-9.5, -7.));
	score->attach<TextComponent>("0 - 0");
	score->commit();
	
	sf::Clock timer;
	
	sf::Time lastMillis = timer.getElapsedTime();
	InputController inputController(0);

	auto scoreManager = std::make_shared<ScoreManager>(ball, score);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		sf::Time currentMillis = timer.getElapsedTime();
		EntityW::Time delta((currentMillis - lastMillis).asMicroseconds());
		lastMillis = currentMillis;

		// low level systems
		inputController.process();

		//event bus
		EntityW::EventDispatcher::get().process();

		// logic systems
		inputSystem->Process(delta);
		movementSystem->Process(delta);
		attachSystem->Process(delta);
		collisionSystem->Process(delta);

		physicsSystem->Process(delta);

		//rendering pipeline
		window.clear();
		
		renderSystem->Process(delta);
		textRenderingSystem->Process(delta);
		window.display();
		
	}
	
	return 0;
}


