#include "Entity.h"
#include "../Components.h"

namespace EntityW {
	int Entity::currentEntityId = 0;
	std::map<long, std::shared_ptr<Entity>> Entity::entities;

	bool Entity::hasComponents(ComponentList componentList) {
		return (this->componentList & componentList) == componentList;
	};

	sol::object Entity::scriptGet(TypeId component, sol::this_state s)
	{
		sol::state_view lua(s);
		if (components.find(component) != components.end())
		{
			if (component == ComponentTypeId <TextComponent>())
			{
				return sol::make_object(lua, std::static_pointer_cast<TextComponent>(components[component]));
			}
			else if (component == ComponentTypeId <TransformComponent>())
			{
				return sol::make_object(lua, std::static_pointer_cast<TransformComponent>(components[component]));
			}
			else if (component == ComponentTypeId <CollisionComponent>())
			{
				return sol::make_object(lua, std::static_pointer_cast<CollisionComponent>(components[component]));
			}
			else if (component == ComponentTypeId <VelocityComponent>())
			{
				return sol::make_object(lua, std::static_pointer_cast<VelocityComponent>(components[component]));
			}
		}
		else
		{
			return scriptComponents[component];
		}
	}

	bool Entity::scriptHas(TypeId component)
	{
		return componentList[component];
	}

	void Entity::scriptAttach(TypeId componentId, sol::table component)
	{

		if (componentId == ComponentTypeId<TextComponent>())
		{
			attach<TextComponent>(std::string(component["text"]));
		}
		else
		{
			componentList.set(componentId);
			scriptComponents[componentId] = component;
			if (commited)
			{
				EventDispatcher::get().emitNow<ScriptComponentAttachedEvent>(thisPtr, component);
			}
		}
	}

	void Entity::scriptDetach(TypeId componentId)
	{
		componentList.reset(componentId);
		scriptComponents.erase(componentId);

		if (commited)
		{
			EventDispatcher::get().emitNow<ScriptComponentDetachedEvent>(thisPtr);
		}
	}
}