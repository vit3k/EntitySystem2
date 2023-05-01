#include "ScriptManager.h"
#include "Components.h"
#include "Event.h"
#include "CollisionSystem.h"
#include "EntityW/ClassTypeId.h"
#include <algorithm>
#include "Input.h"
#include "Animation.h"
#include "Engine.h"

ScriptManager::ScriptManager()
{
}


ScriptManager::~ScriptManager()
{
}

EntityW::TypeId ScriptManager::registerComponent(sol::table table, std::string name)
{
	auto id = EntityW::ScriptComponentTypeId();
	logger.log("Registering component " + name + " with id " + std::to_string(id));
	name[0] = toupper(name[0]);
	table[name] = id;
	scriptComponentsMap[name] = id;
	return id;

}

EntityW::TypeId ScriptManager::registerEvent(sol::table table, std::string name)
{
	auto id = EntityW::ScriptEventTypeId();
	logger.log("Registering event " + name + " with id " + std::to_string(id));
	name[0] = toupper(name[0]);
	table[name] = id;
	return id;
}

void ScriptManager::subscribe(EntityW::TypeId eventTypeId, sol::function listener)
{
	logger.log("Subscribe to " + std::to_string(eventTypeId));
	EntityW::EventDispatcher::get().scriptSubscribe(eventTypeId, listener);
}


void ScriptManager::subscribe(EntityW::TypeId eventTypeId, sol::function listener, sol::table self)
{
	logger.log("Subscribe to " + std::to_string(eventTypeId) + " with object");
	EntityW::EventDispatcher::get().scriptSubscribe(eventTypeId, listener, self);
}

EntityW::EntitySp ScriptManager::createEntityDirect(sol::table entityData)
{
    auto entity = EntityW::Entity::create();
    entityData.for_each([entity, this](sol::object key, sol::object value) {

        //if (value.is<EntityW::ComponentSp>())
        //{
            auto component = value.as<EntityW::ComponentSp>();
            entity->attach(component);
        //}
        /*else
        {
            auto componentName = key.as<std::string>();
            auto componentNameCase = componentName;
            componentNameCase[0] = toupper(componentNameCase[0]);
            if (scriptComponentsMap.find(componentNameCase) == scriptComponentsMap.end())
            {
                registerComponent(lua["internal_Components"], componentNameCase);
            }
            entity->scriptAttach(scriptComponentsMap[componentNameCase], value.as<sol::object>());
        }*/
    });
    entity->commit();
    return entity;
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
		else if (componentName == "sprite")
		{
            logger.log("creating sprite");
			auto table = value.as<sol::table>();
			auto spriteComponent = SpriteComponent::create(table["path"]);
            spriteComponent->scale.x = table["scale"]["x"];
            spriteComponent->scale.y = table["scale"]["y"];
            entity->attach(spriteComponent);
		}
		else
		{
			auto componentNameCase = componentName;
			componentNameCase[0] = toupper(componentNameCase[0]);
			if (scriptComponentsMap.find(componentNameCase) == scriptComponentsMap.end())
			{
				registerComponent(lua["Components"], componentNameCase);
			}
			entity->scriptAttach(scriptComponentsMap[componentNameCase], value.as<sol::object>());
		}
	});
	entity->commit();
	return entity;
}

void ScriptManager::init()
{
	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::os);
	lua["package"]["path"] = "./scripts/?.lua";
	lua.script("print('LUA online')");
	lua.new_usertype<Vector2>("Vector2",
		sol::constructors<Vector2(double, double)>(),
		"x", &Vector2::x,
		"y", &Vector2::y
		);

	//lua.new_usertype<EntityW::Component>("Component");
	
	lua.new_usertype<TransformComponent>("TransformComponent",
		//sol::constructors<TransformComponent(Vector2)>(),
		sol::factories(TransformComponent::create),
		"position", &TransformComponent::position,
		sol::base_classes, sol::bases<EntityW::Component<TransformComponent>>()
		);

	lua.new_usertype<EntityW::Entity>("Entity",
		sol::constructors<EntityW::Entity()>(),
		"id", &EntityW::Entity::id,
		"get", &EntityW::Entity::scriptGet,
		"has", &EntityW::Entity::scriptHas,
		"attach", &EntityW::Entity::scriptAttach,
		"detach", &EntityW::Entity::scriptDetach,
		"attachDirect", &EntityW::Entity::attachDirect
		);

	lua.new_usertype<TextComponent>("TextComponent",
		sol::constructors<TextComponent(std::string)>(),
		"text", &TextComponent::text,
		sol::base_classes, sol::bases<EntityW::Component<TextComponent>>()
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

	lua.new_usertype<sf::Vector2f>("Vector2f",
		sol::constructors < sf::Vector2f(float, float) >()
		);

	lua.new_usertype<sf::Shape>("Shape",
		"setFillColor", &sf::RectangleShape::setFillColor
		);

	lua.new_usertype<sf::RectangleShape>("RectangleShape",
		sol::constructors<sf::RectangleShape(sf::Vector2f)>(),
		"setFillColor", &sf::RectangleShape::setFillColor,
		sol::base_classes, sol::base_list<sf::Shape>()
		);

	lua.new_usertype<sf::CircleShape>("CircleShape",
		sol::constructors<sf::CircleShape(float)>(),
		sol::base_classes, sol::base_list<sf::Shape>()
		);

	lua.new_usertype<RenderComponent>("RenderComponent",
		sol::constructors<RenderComponent(std::shared_ptr<sf::Shape>)>(),
		sol::base_classes, sol::bases<EntityW::Component<RenderComponent>>()

		);

	lua.new_usertype<VelocityComponent>("VelocityComponent",
		sol::constructors<VelocityComponent(Vector2, float)>(),
		"velocity", &VelocityComponent::velocity,
		"bounciness", &VelocityComponent::bounciness,
		sol::base_classes, sol::bases<EntityW::Component<VelocityComponent>>()
		);

	lua.new_usertype<CollisionShape>("CollisionShape");

	lua.new_usertype<RectCollisionShape>("RectCollisionShape",
		sol::constructors<RectCollisionShape(float, float)>(),
		"width", &RectCollisionShape::width,
		"height", &RectCollisionShape::height,
		sol::base_classes, sol::bases<CollisionShape>()
		);

	lua.new_usertype<CircleCollisionShape>("CircleCollisionShape",
		sol::constructors<CircleCollisionShape(float)>(),
		"center", &CircleCollisionShape::center,
		sol::base_classes, sol::bases<CollisionShape>()
		);

	lua.new_usertype<CollisionComponent>("CollisionComponent",
		sol::constructors<CollisionComponent(std::shared_ptr<CollisionShape>)>(),
		"shape", &CollisionComponent::shape,
		sol::base_classes, sol::bases<EntityW::Component<CollisionComponent>>()
		);

	lua.new_usertype<PhysicsComponent>("PhysicsComponent",
		sol::constructors<PhysicsComponent(float, float, Vector2)>(),
		"constraints", &PhysicsComponent::constraints,
		"bounciness", &PhysicsComponent::bounciness,
		"mass", &PhysicsComponent::mass,
		"invertedMass", &PhysicsComponent::invertedMass,
		sol::base_classes, sol::bases<EntityW::Component<PhysicsComponent>>()
		);

	lua.new_usertype<SpriteSheet>("SpriteSheet",
		//sol::constructors<SpriteSheet(std::string, int, int)>(),
		sol::factories(SpriteSheet::create),
		"load", &SpriteSheet::load
	);

	lua.new_usertype<SpriteComponent>("SpriteComponent", 
		sol::factories(SpriteComponent::create),
        "scale", &SpriteComponent::scale,
		sol::base_classes, sol::bases<EntityW::Component<SpriteComponent>>()
	);

	//lua["loadSpriteSheet"] = [](std::string path, int frameWidth, int frameHeight) -> std::shared_ptr<SpriteSheet> { return std::make_shared<SpriteSheet>(path, frameWidth, frameHeight); };


	lua.new_enum("sfColor",
		"Green", sf::Color::Green,
		"Red", sf::Color::Red,
		"White", sf::Color::White
	);
	lua.new_enum("Shapes",
		"Rectangle", ShapeType::Rectangle,
		"Circle", ShapeType::Circle
	);

	lua.new_usertype<EntityW::TypeId>("TypeId");

	lua["internal_Events"] = lua.create_table_with(
		"Collision", EntityW::EventTypeId<CollisionEvent>(),
		"MoveUp", EntityW::EventTypeId<MoveUpEvent>(),
		"MoveDown", EntityW::EventTypeId<MoveDownEvent>(),
		"Started", EntityW::EventTypeId<StartedEvent>(),
		"LaunchBall", EntityW::EventTypeId<LaunchBallEvent>()
	);

	auto eventsMetatable = lua.create_table();
	eventsMetatable.set_function(sol::meta_function::index, &ScriptManager::registerEvent, this);
	lua["internal_Events"][sol::metatable_key] = eventsMetatable;

	lua["internal_Components"] = lua.create_table_with(
		"Transform", EntityW::ComponentTypeId<TransformComponent>(),
		"Text", EntityW::ComponentTypeId<TextComponent>(),
		"Render", EntityW::ComponentTypeId<RenderComponent>(),
		"Collision", EntityW::ComponentTypeId<CollisionComponent>(),
		"Physics", EntityW::ComponentTypeId<PhysicsComponent>(),
		"Velocity", EntityW::ComponentTypeId<VelocityComponent>(),
		"Sprite", EntityW::ComponentTypeId<SpriteComponent>()
	);

	auto metatable = lua.create_table();
	metatable.set_function(sol::meta_function::index, &ScriptManager::registerComponent, this);
	lua["internal_Components"][sol::metatable_key] = metatable;

	lua.new_usertype<EntityW::Time>("Time",
			"asSeconds", &EntityW::Time::asSeconds
		);
	lua.new_usertype<Input>("internal_Input",
        "isActionPressed", &Input::isActionPressed,
        "mapAction", &Input::mapAction
	);
	lua["internal_Key"] = lua.create_table_with(
		"Up", sf::Keyboard::Key::Up,
		"Down", sf::Keyboard::Key::Down,
		"W", sf::Keyboard::Key::W,
		"S", sf::Keyboard::Key::S,
        "A", sf::Keyboard::Key::A,
        "D", sf::Keyboard::Key::D);

	sol::table glmTable = lua.create_named_table("glm");
	glmTable.set_function("normalize", &ScriptManager::glmNormalize, this);

	glmTable.set_function("clamp", [] (double input, double min, double max) {
		return glm::clamp(input, min, max);
	});

	lua.set_function("internal_createEntity", &ScriptManager::createEntity, this);
    lua.set_function("internal_createEntityDirect", &ScriptManager::createEntityDirect, this);
	lua.set_function("internal_subscribe", sol::resolve<void(EntityW::TypeId, sol::function)>(&ScriptManager::subscribe), this);
	lua.set_function("internal_subscribeForObject", sol::resolve<void(EntityW::TypeId, sol::function, sol::table)>(&ScriptManager::subscribe), this);
	lua.set_function("internal_registerSystem", &ScriptManager::registerSystem, this);
	//lua.set_function("import", &ScriptManager::importModule, this);
	lua.set_function("internal_emit", &ScriptManager::emit, this);
	lua.set_function("internal_clearWorld", &ScriptManager::clearWorld, this);
	lua.new_usertype<EntityW::ScriptSystem>("System",
		sol::constructors<EntityW::ScriptSystem(sol::table)>()
	);

	lua.new_usertype<StartedEvent>("StartedEvent");

	lua.new_usertype<MoveUpEvent>("MoveUpEvent",
		"controller", &MoveUpEvent::controller
	);

	lua.new_usertype<MoveDownEvent>("MoveDownEvent",
		"controller", &MoveDownEvent::controller
	);

	lua.new_usertype<LaunchBallEvent>("LaunchBallEvent",
		"controller", &LaunchBallEvent::controller
	);

    lua.set_function("internal_EngineInit", &ScriptManager::engineInit, this);
}

void ScriptManager::run(std::string name)
{
	lua.script_file(name);
}

Vector2 ScriptManager::glmNormalize(Vector2 vec)
{
	return glm::normalize(vec);
}

sol::table ScriptManager::registerSystem(sol::table script)
{
	auto system = std::make_shared<EntityW::ScriptSystem>(script);
	systems.push_back(system);
	return script;
}

void ScriptManager::process(EntityW::Time deltaTime)
{
	for (auto system : systems)
	{
		system->Process(deltaTime);
	}
}

sol::object ScriptManager::importModule(std::string modulePath)
{
	return lua.require_file(modulePath, "scripts/" + modulePath + ".lua", false);
}

void ScriptManager::clearWorld()
{
	EntityW::EventDispatcher::get().scriptClear();
	EntityW::Entity::clear();
	systems.clear();
}

void ScriptManager::emit(EntityW::TypeId type, sol::object data)
{
	EntityW::EventDispatcher::get().scriptEmit(type, data);
}

void ScriptManager::close()
{
	lua.stack_clear();
}

void ScriptManager::engineInit(sol::table config) {
    //auto configuration = config.as<Configuration>();
    Configuration configuration;
    configuration.frameLimit = config["frameLimit"];
    configuration.window.width = config["window"]["width"];
    configuration.window.height = config["window"]["height"];
    configuration.window.title = config["window"]["title"];

    Engine::getInstance()->init(configuration);
}
