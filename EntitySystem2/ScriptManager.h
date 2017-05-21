#pragma once
#include "EntityW\Entity.h"
#include "sol.hpp"
#include "Vector2.h"
#include <memory>
#include "EntityW\System.h"

class ScriptManager
{
private:
	std::map<std::string, EntityW::TypeId> scriptComponentsMap;
	sol::state lua;
	std::vector<std::shared_ptr<EntityW::ScriptSystem>> systems;
	Logger logger = Logger::get("ScriptManager");
public:
	ScriptManager();
	~ScriptManager();
	EntityW::TypeId registerComponent(sol::table table, std::string name);
	void subscribe(EntityW::TypeId eventTypeId, sol::function listener);
	void subscribe(EntityW::TypeId eventTypeId, sol::function listener, sol::table self);
	EntityW::EntitySp createEntity(sol::table entityData);
	void init();
	Vector2 glmNormalize(Vector2 vec);
	void run(std::string name);
	sol::table registerSystem(sol::table script);
	void process(EntityW::Time deltaTime);
	sol::object importModule(std::string modulePath);
	void clearWorld();
};
/*
class ComponentsMap {
	std::map<std::string, EntityW::TypeId> componentsMap;

public:
	/*EntityW::TypeId getComponent(std::string name, sol::this_state state)
	{
		//if (componentsMap)
	}

	ComponentsMap()
	{
		componentsMap["Transform"] = EntityW::ComponentTypeId<TransformComponent>();
		componentsMap["Text"] = EntityW::ComponentTypeId<TextComponent>();
		componentsMap["Render"] = EntityW::ComponentTypeId<RenderComponent>();
		componentsMap["Collision"] = EntityW::ComponentTypeId<CollisionComponent>();
		componentsMap["Physics"] = EntityW::ComponentTypeId<PhysicsComponent>();
		componentsMap["Velocity"] = EntityW::ComponentTypeId<VelocityComponent>();
	}
};
*/