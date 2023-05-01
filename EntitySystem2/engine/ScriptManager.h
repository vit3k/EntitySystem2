#pragma once
#include "EntityW/Entity.h"
#include <sol/sol.hpp>
#include "Vector2.h"
#include <memory>
#include "EntityW/System.h"
#include "EntityW/ClassTypeId.h"

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
    EntityW::EntitySp createEntityDirect(sol::table entityData);
	void init();
	Vector2 glmNormalize(Vector2 vec);
	void run(std::string name);
	sol::table registerSystem(sol::table script);
	void process(EntityW::Time deltaTime);
	sol::object importModule(std::string modulePath);
	void clearWorld();
	EntityW::TypeId registerEvent(sol::table table, std::string name);
	void emit(EntityW::TypeId type, sol::object data);
	void close();
    void engineInit(sol::table config);
};
