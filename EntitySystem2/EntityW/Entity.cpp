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
			if (component == ComponentTypeId<ScoringSurfaceComponent>()) 
			{
				auto scoring = std::static_pointer_cast<ScoringSurfaceComponent>(components[component]);
				return sol::make_object(lua, scoring);
			}
			else if (component == ComponentTypeId <TextComponent> ())
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
			else if (component == ComponentTypeId <AttachComponent>())
			{
				return sol::make_object(lua, std::static_pointer_cast<AttachComponent>(components[component]));
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

	void Entity::scriptAttach(TypeId componentId, sol::object component)
	{
		componentList.set(componentId);

		if (component.is<Component>())
		{
			auto comp = std::shared_ptr<Component>(&component.as<Component>()); // this sucks
			components[componentId] = comp;
			if (commited) 
			{
				EventDispatcher::get().emitNow<ComponentAttachedEvent>(thisPtr, comp);
			}
		}
		else
		{
			scriptComponents[componentId] = component;
		}
	}
}