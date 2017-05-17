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
public:
	ScriptManager();
	~ScriptManager();
	EntityW::TypeId registerComponent(std::string name);
	void subscribe(EntityW::TypeId eventTypeId, sol::function listener);
	void subscribe(EntityW::TypeId eventTypeId, sol::function listener, sol::table self);
	EntityW::EntitySp createEntity(sol::table entityData);
	void init();
	Vector2 glmNormalize(Vector2 vec);
	void run(std::string name);
	sol::table registerSystem(sol::table script, sol::variadic_args args);
	void process(EntityW::Time deltaTime);
	sol::object importModule(std::string modulePath);
	void clearWorld();
};

