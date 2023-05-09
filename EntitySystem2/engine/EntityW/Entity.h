#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include "Component.h"
#include "EventDispatcher.h"
#include <sol/sol.hpp>

namespace EntityW {
	class EntityCreatedEvent;
	class ComponentAttachedEvent;
	class EntityRemovedEvent;
	class Entity;

	class Entity {
	protected:
		std::map<TypeId, ComponentSp> components;
		std::map<TypeId, sol::object> scriptComponents;

		ComponentList componentList;
		static int currentEntityId;
		bool commited = false;
		std::shared_ptr<Entity> thisPtr;
		static std::map<long, std::shared_ptr<Entity>> entities;
	public:
		void setPointer(std::shared_ptr<Entity> ptr) {
			thisPtr = ptr;
		}
		static int getNextEntityId() { return currentEntityId++; }
		std::string name;

		template <typename T, typename... Args>
		std::shared_ptr<T> attach(Args... args);

		void attach(ComponentSp component);
		void attachDirect(ComponentSp component)
		{
			attach(component);
		};

		template <typename T>
		std::shared_ptr<T> get();

        ComponentSp get(TypeId type);

		//sol::objecty
		bool hasComponents(ComponentList componentList);
		template <typename T>
		bool has() {
			return componentList[ComponentTypeId<T>()];
		}

        bool has(TypeId id) {
            return componentList[id];
        }

		long id;
		Entity() {
			id = getNextEntityId();
		};

		static std::shared_ptr<Entity> create() {
			auto entity = std::shared_ptr<Entity>(new Entity());
			entity->setPointer(entity);
			return entity;
		}

		std::shared_ptr<Entity> commit() {
			EventDispatcher::get().emitNow<EntityCreatedEvent>(thisPtr);
			entities[id] = thisPtr;
			commited = true;
			return thisPtr;
		}

		bool scriptHas(TypeId component);
		sol::object scriptGet(TypeId component, sol::this_state s);
		void scriptAttach(TypeId componentId, sol::table component);
		void scriptDetach(TypeId componentId);
		
		static void clear()
		{
			//EventDispatcher::get().emitNow<ClearWorldEvent>();
			for (auto entity : entities)
			{
				EventDispatcher::get().emitNow<EntityRemovedEvent>(entity.second);
			}
			entities.clear();
		}
	};

	typedef std::shared_ptr<Entity> EntitySp;

	template <typename T, typename... Args>
	std::shared_ptr<T> Entity::attach(Args... args)
	{
		static_assert(std::is_base_of<Component<T>, T>(), "T is not a component, cannot add T to entity");
		auto component = std::make_shared<T>(std::forward<Args>(args)...);
		components[ComponentTypeId<T>()] = component;
		componentList.set(ComponentTypeId<T>());
		if (commited) {
			EventDispatcher::get().emitNow<ComponentAttachedEvent>(thisPtr, component);
		}
		return component;
	}

	template<typename T>
	std::shared_ptr<T> Entity::get()
	{
		static_assert(std::is_base_of<Component<T>, T>(), "T is not a component, cannot get it from entity");
		return std::static_pointer_cast<T>(components[ComponentTypeId<T>()]);
	}

	class EntityCreatedEvent : public Event<EntityCreatedEvent> {
	public:
		EntitySp entity;
		EntityCreatedEvent(EntitySp entity) : entity(entity) {}
	};

	class EntityRemovedEvent : public Event<EntityRemovedEvent> {
	public:
		EntitySp entity;
		EntityRemovedEvent(EntitySp entity) : entity(entity) {}
	};

	class ClearWorldEvent : public Event<ClearWorldEvent> {

	};

	class ComponentAttachedEvent : public Event<ComponentAttachedEvent> {
	public:
		EntitySp entity;
		ComponentSp component;
		ComponentAttachedEvent(EntitySp entity, ComponentSp component) : entity(entity), component(component) {}
	};

	class ScriptComponentAttachedEvent : public Event<ScriptComponentAttachedEvent> {
	public:
		EntitySp entity;
		sol::table component;

		ScriptComponentAttachedEvent(EntitySp entity, sol::table component) : entity(entity), component(component) {}
	};

	class ScriptComponentDetachedEvent : public Event<ScriptComponentDetachedEvent> {
	public:
		EntitySp entity;

		ScriptComponentDetachedEvent(EntitySp entity) : entity(entity) {}
	};
}


#endif