#include "ScriptManager.h"
#include "Components.h"
#include "Event.h"
#include "CollisionSystem.h"

ScriptManager::ScriptManager()
{
}


ScriptManager::~ScriptManager()
{
}

EntityW::TypeId ScriptManager::registerComponent(std::string name)
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

void ScriptManager::subscribe(EntityW::TypeId eventTypeId, sol::function listener)
{
	EntityW::EventDispatcher::get().scriptSubscribe(eventTypeId, listener);
}

EntityW::EntitySp ScriptManager::createEntity(sol::table entityData)
{
	auto entity = EntityW::Entity::create();
	entityData.for_each([entity, this](sol::object key, sol::object value) {
		auto componentName = key.as<std::string>();
		if (componentName == "transform")
		{
			auto position = value.as<sol::table>();
			entity->attach<TransformComponent>(Vector2(position["x"], position["y"]));
		}
		else if (componentName == "text")
		{
			entity->attach<TextComponent>(value.as<std::string>());
		}
		else if (componentName == "render")
		{
			auto shapeTable = value.as<sol::table>()["shape"];
			if (shapeTable["type"] == Rectangle)
			{
				auto shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(shapeTable["width"], shapeTable["height"]));
				shape->setFillColor(shapeTable["color"]);
				entity->attach<RenderComponent>(std::static_pointer_cast<sf::Shape>(shape));
			}
			if (shapeTable["type"] == Circle)
			{
				float radius = shapeTable["radius"];
				auto shape = std::make_shared<sf::CircleShape>(radius);
				shape->setFillColor(shapeTable["color"]);
				entity->attach<RenderComponent>(std::static_pointer_cast<sf::Shape>(shape));
			}
		}
		else if (componentName == "velocity")
		{
			auto table = value.as<sol::table>();
			entity->attach<VelocityComponent>(Vector2(table["velocity"]["x"], table["velocity"]["y"]), table.get<float>("bounciness"));
		}
		else if (componentName == "input")
		{
			entity->attach<InputComponent>(value.as<int>());
		}
		else if (componentName == "collision")
		{
			auto shapeTable = value.as<sol::table>()["shape"];
			if (shapeTable["type"] == Rectangle)
			{
				auto shape = std::make_shared<RectCollisionShape>(shapeTable["width"], shapeTable["height"]);
				entity->attach<CollisionComponent>(std::static_pointer_cast<CollisionShape>(shape));
			}
			if (shapeTable["type"] == Circle)
			{
				float radius = shapeTable["radius"];
				auto shape = std::make_shared<CircleCollisionShape>(radius);
				entity->attach<CollisionComponent>(std::static_pointer_cast<CollisionShape>(shape));
			}
		}
		else if (componentName == "physics")
		{
			auto table = value.as<sol::table>();
			entity->attach<PhysicsComponent>(table["bounciness"], table["mass"], Vector2(table["constraints"]["x"], table["constraints"]["y"]));
		}
		else if (componentName == "scoringSurface")
		{
			auto table = value.as<sol::table>();
			entity->attach<ScoringSurfaceComponent>(table["playerId"], table.get<EntityW::EntitySp>("paddle"));
		}
		else if (componentName == "attach")
		{
			auto table = value.as<sol::table>();
			entity->attach<AttachComponent>(table.get<TransformComponentSp>("parent"), Vector2(table["relative"]["x"], table["relative"]["y"]));
		}
		else
		{
			entity->scriptAttach(scriptComponentsMap[componentName], value.as<sol::object>());
		}
	});
	entity->commit();
	return entity;
}

void ScriptManager::init()
{
	lua.open_libraries(sol::lib::base);
	lua.script("print('LUA online')");
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
		sol::constructors < sf::Vector2f(float, float) >()
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
		sol::constructors<RenderComponent(std::shared_ptr<sf::Shape>)>(),
		sol::base_classes, sol::bases<EntityW::Component>()

		);

	lua.new_usertype<InputComponent>("InputComponent",
		sol::constructors<InputComponent(int)>(),
		sol::base_classes, sol::bases<EntityW::Component>()
		);

	lua.new_usertype<VelocityComponent>("VelocityComponent",
		sol::constructors<VelocityComponent(Vector2, float)>(),
		"velocity", &VelocityComponent::velocity,
		"bounciness", &VelocityComponent::bounciness,
		sol::base_classes, sol::bases<EntityW::Component>()
		);

	lua.new_usertype<CollisionShape>("CollisionShape",
		"width", &RectCollisionShape::width,
		"height", &RectCollisionShape::height,
		"center", &CircleCollisionShape::center);

	lua.new_usertype<RectCollisionShape>("RectCollisionShape",
		sol::constructors<RectCollisionShape(float, float)>(),
		"width", &RectCollisionShape::width,
		"height", &RectCollisionShape::height,
		sol::base_classes, sol::bases<CollisionShape>()
		);

	lua.new_usertype<CircleCollisionShape>("CircleCollisionShape",
		sol::constructors<CircleCollisionShape(float)>(),
		sol::base_classes, sol::bases<CollisionShape>()
		);

	lua.new_usertype<CollisionComponent>("CollisionComponent",
		sol::constructors<CollisionComponent(std::shared_ptr<CollisionShape>)>(),
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
	lua.new_enum("Shapes",
		"Rectangle", ShapeType::Rectangle,
		"Circle", ShapeType::Circle
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

	lua.new_usertype<EntityW::Time>("Time",
			"asSeconds", &EntityW::Time::asSeconds
		);
	sol::table glm = lua.create_named_table("glm");
	glm.set_function("normalize", &ScriptManager::glmNormalize, this);

	lua.set_function("createEntity", &ScriptManager::createEntity, this);
	lua.set_function("subscribe", &ScriptManager::subscribe, this);
	lua.set_function("registerComponent", &ScriptManager::registerComponent, this);
	lua.set_function("registerSystem", &ScriptManager::registerSystem, this);
	lua.new_usertype<EntityW::ScriptSystem>("System",
		sol::constructors<EntityW::ScriptSystem(sol::table, sol::variadic_args)>()
	);
}

void ScriptManager::run(std::string name)
{
	lua.script_file(name);
}

Vector2 ScriptManager::glmNormalize(Vector2 vec)
{
	return glm::normalize(vec);
}

std::shared_ptr<EntityW::ScriptSystem> ScriptManager::registerSystem(sol::table script, sol::variadic_args args)
{
	auto system = std::make_shared<EntityW::ScriptSystem>(script, args);
	systems.push_back(system);
	return system;
}

void ScriptManager::process(EntityW::Time deltaTime)
{
	for (auto system : systems)
	{
		system->Process(deltaTime);
	}
}