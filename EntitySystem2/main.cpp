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
#include <algorithm>
#include "EntityW\ClassTypeId.h"

std::map<std::string, EntityW::TypeId> scriptComponentsMap;

EntityW::EntitySp createEntity(sol::table entityData)
{
	auto entity = EntityW::Entity::create();
	entityData.for_each([entity](sol::object key, sol::object value) {
		auto componentName = key.as<std::string>();
		if (componentName == "transform")
		{
			entity->attach<TransformComponent>(value.as<Vector2>());
		}
		else if (componentName == "text")
		{
			entity->attach<TextComponent>(value.as<std::string>());
		}
		else if (componentName == "render")
		{
			auto shape = value.as<sol::table>().get<sf::Shape*>("shape");
			entity->attach<RenderComponent>(shape);
		}
		else if (componentName == "velocity")
		{
			auto table = value.as<sol::table>();
			entity->attach<VelocityComponent>(table.get<Vector2>("velocity"), table.get<float>("bounciness"));
		}
		else if (componentName == "input")
		{
			entity->attach<InputComponent>(value.as<int>());
		}
		else if (componentName == "collision")
		{
			auto table = value.as<sol::table>();
			entity->attach<CollisionComponent>(table.get<CollisionShape*>("shape"));
		}
		else if (componentName == "physics")
		{
			auto table = value.as<sol::table>();
			entity->attach<PhysicsComponent>(table.get<float>("bounciness"), table.get<float>("mass"), table.get<Vector2>("constraints"));
		}
		else if (componentName == "scoringSurface")
		{
			auto table = value.as<sol::table>();
			entity->attach<ScoringSurfaceComponent>(table.get<int>("playerId"), table.get<EntityW::EntitySp>("paddle"));
		}
		else if (componentName == "attach")
		{
			auto table = value.as<sol::table>();
			entity->attach<AttachComponent>(table.get<TransformComponentSp>("parent"), table.get<Vector2>("relative"));
		}
		else
		{
			entity->scriptAttach(scriptComponentsMap[componentName], value.as<sol::object>());
		}
	});
	entity->commit();
	return entity;
}

void subscribe(EntityW::TypeId eventTypeId, sol::function listener)
{
	EntityW::EventDispatcher::get().scriptSubscribe(eventTypeId, listener);
}

EntityW::TypeId registerComponent(std::string name)
{
	auto id = EntityW::ScriptComponentTypeId();
	if (scriptComponentsMap.find(name) != scriptComponentsMap.end())
	{
		return scriptComponentsMap[name];
	}
	else
	{
		scriptComponentsMap[name] = id;
		return id;
	}
}
/*
std::vector < std::shared_ptr<EntityW::ScriptSystem>> scriptSystems;

void registerSystem(std::string name, sol::table script)
{
	auto system = std::make_shared<EntityW::ScriptSystem>(script);
	scriptSystems.push_back(system);
}
*/

Vector2 glmNormalize(Vector2 vec)
{
	return glm::normalize(vec);
}

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

	sol::state lua;
	
	lua.open_libraries(sol::lib::base);
	lua.script("print('bark bark bark!')");

	lua.new_usertype<Vector2>("Vector2",
		sol::constructors<Vector2(double, double)>(),
		"x", &Vector2::x,
		"y", &Vector2::y
	);

	lua.new_simple_usertype<EntityW::Component>("Component");

	lua.new_usertype<TransformComponent>("TransformComponent",
		sol::constructors<TransformComponent(Vector2)>(),
		"position", &TransformComponent::position,
		sol::base_classes, sol::bases<EntityW::Component>()
	);

	lua.new_usertype<EntityW::Entity>("Entity",
		sol::constructors<EntityW::Entity()>(),
		"id", &EntityW::Entity::id,
		"get", &EntityW::Entity::scriptGet,
		"has", &EntityW::Entity::scriptHas,
		"attach", &EntityW::Entity::scriptAttach
	);

	lua.new_usertype<TextComponent>("TextComponent",
		sol::constructors<TextComponent(std::string)>(),
		"text", &TextComponent::text,
		sol::base_classes, sol::bases<EntityW::Component>()
	);

	lua.new_usertype<Collision>("Collision",
		sol::constructors<Collision()>(),
		"occured", &Collision::occured,
		"depth", &Collision::depth,
		"normal", &Collision::normal
	);

	lua.new_usertype<CollisionEvent>("CollisionEvent",
		sol::constructors<CollisionEvent(Collision, EntityW::EntitySp, EntityW::EntitySp)>(),
		"entity1", &CollisionEvent::entity1,
		"entity2", &CollisionEvent::entity2,
		"collision", &CollisionEvent::collision
	);

	lua.new_usertype<ScoringSurfaceComponent>("ScoringSurfaceComponent",
		sol::constructors < ScoringSurfaceComponent(int, EntityW::EntitySp)>(),
		"paddle", &ScoringSurfaceComponent::paddle,
		"playerId", &ScoringSurfaceComponent::playerId,
		sol::base_classes, sol::bases<EntityW::Component>()
	);

	lua.new_usertype<AttachComponent>("AttachComponent",
		sol::constructors<AttachComponent(TransformComponentSp, Vector2)>(),
		"parentTransform", &AttachComponent::parentTransform,
		"relativePosition", &AttachComponent::relativePosition,
		sol::base_classes, sol::bases<EntityW::Component>()
	);

	lua.new_simple_usertype<sf::Vector2f>("Vector2f",
		sol::constructors < sf::Vector2f(float, float) > ()
	);

	lua.new_simple_usertype<sf::Shape>("Shape",
		"setFillColor", &sf::RectangleShape::setFillColor
	);

	lua.new_simple_usertype<sf::RectangleShape>("RectangleShape",
		sol::constructors<sf::RectangleShape(sf::Vector2f)>(),
		"setFillColor", &sf::RectangleShape::setFillColor,
		sol::base_classes, sol::base_list<sf::Shape>()
	);

	lua.new_simple_usertype<sf::CircleShape>("CircleShape",
		sol::constructors<sf::CircleShape(float)>(),
		sol::base_classes, sol::base_list<sf::Shape>()
		);

	lua.new_usertype<RenderComponent>("RenderComponent",
		sol::constructors<RenderComponent(sf::Shape*)>(),
		sol::base_classes, sol::bases<EntityW::Component>()
		
		);

	lua.new_usertype<InputComponent>("InputComponent",
		sol::constructors<InputController(int)>(),
		sol::base_classes, sol::bases<EntityW::Component>()
	);

	lua.new_usertype<VelocityComponent>("VelocityComponent",
		sol::constructors<VelocityComponent(Vector2, float)>(),
		"velocity", &VelocityComponent::velocity,
		"bounciness", &VelocityComponent::bounciness,
		sol::base_classes, sol::bases<EntityW::Component>()
	);

	lua.new_usertype<CollisionShape>("CollisionShape");
	lua.new_usertype<RectCollisionShape>("RectCollisionShape",
		sol::constructors<RectCollisionShape(float, float)>(),
		sol::base_classes, sol::bases<CollisionShape>()
	);
	
	lua.new_usertype<CircleCollisionShape>("CircleCollisionShape",
		sol::constructors<CircleCollisionShape(float)>(),
		sol::base_classes, sol::bases<CollisionShape>()
		);

	lua.new_usertype<CollisionComponent>("CollisionComponent",
		sol::constructors<CollisionComponent(CollisionShape*)>(),
		"shape", &CollisionComponent::shape,
		sol::base_classes, sol::bases<EntityW::Component>()
	);

	lua.new_usertype<PhysicsComponent>("PhysicsComponent",
		sol::constructors<PhysicsComponent(float, float, Vector2)>(),
		"constraints", &PhysicsComponent::constraints,
		"bounciness", &PhysicsComponent::bounciness,
		"mass", &PhysicsComponent::mass,
		"invertedMass", &PhysicsComponent::invertedMass,
		sol::base_classes, sol::bases<EntityW::Component>()
	);

	lua.new_enum("sfColor",
		"Green", sf::Color::Green,
		"Red", sf::Color::Red,
		"White", sf::Color::White
	);

	lua["Events"] = lua.create_table_with(
		"Collision", EntityW::EventTypeId<CollisionEvent>()
	);

	lua["Components"] = lua.create_table_with(
		"Transform", EntityW::ComponentTypeId<TransformComponent>(),
		"Text", EntityW::ComponentTypeId<TextComponent>(),
		"Render", EntityW::ComponentTypeId<RenderComponent>(),
		"Input", EntityW::ComponentTypeId<InputComponent>(),
		"Collision", EntityW::ComponentTypeId<CollisionComponent>(),
		"Physics", EntityW::ComponentTypeId<PhysicsComponent>(),
		"ScoringSurface", EntityW::ComponentTypeId<ScoringSurfaceComponent>(),
		"Attach", EntityW::ComponentTypeId<AttachComponent>(),
		"Velocity", EntityW::ComponentTypeId<VelocityComponent>()
	);

	sol::table glm = lua.create_named_table("glm");
	glm.set_function("normalize", &glmNormalize);

	lua["createEntity"] = createEntity;
	lua["subscribe"] = subscribe;
	lua["registerComponent"] = registerComponent;

	lua.script_file("test.lua");

	sf::Clock timer;
	
	sf::Time lastMillis = timer.getElapsedTime();
	InputController inputController(0);

	//auto scoreManager = std::make_shared<ScoreManager>(ball, score);

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


