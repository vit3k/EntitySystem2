#ifndef SYSTEM_H
#define SYSTEM_H

#include "Entity.h"
#include "Component.h"
#include <vector>
#include "Time.h"
#include "EventDispatcher.h"
#include "../Logger.h"

namespace EntityW 
{
	class World;

	class BaseSystem 
	{
	protected:
		std::map<long, EntitySp> entities;
		World* world;
		Logger logger;
		virtual std::string getName() { return "BaseSystem";  }
	public:
		virtual void Process(Time deltaTime);
		virtual void ProcessEntity(EntitySp entity, Time deltaTime);
		virtual void OnEntityAdded(EventSp event);
		virtual void OnComponentAttached(EventSp event);
		//void setWorld(World* world) { this->world = world; }
		ComponentList components;
		BaseSystem(ComponentList components) : components(components), logger(Logger::get(getName()))
		{
			EventDispatcher::get().subscribe<EntityCreatedEvent>(EntityW::EventListenerDelegate(this, &BaseSystem::OnEntityAdded));
			EventDispatcher::get().subscribe<ComponentAttachedEvent>(EntityW::EventListenerDelegate(this, &BaseSystem::OnComponentAttached));
		};
	};

	template<class... Args>
	struct TypeList {};

	template<class... Args>
	class System : public BaseSystem 
	{
	public:
		

		template <class... Args>
		ComponentList createComponentList(TypeList<Args...> typeList) { return ComponentList(); };

		template <class T, class... Args>
		ComponentList createComponentList(TypeList<T, Args...> typeList)
		{
			static_assert(std::is_base_of<Component, T>::value, "Invalid component");
			return ComponentList().set(ComponentTypeId<T>()) | createComponentList(TypeList<Args...>());
		};

		System() : BaseSystem(createComponentList(TypeList<Args...>{})) {
		};
	};

	typedef std::shared_ptr<BaseSystem> SystemSp;

	template <class T>
	TypeId SystemTypeId()
	{
		return ClassTypeId<System>::GetTypeId<T>();
	};

	class ScriptSystem : public BaseSystem
	{
		sol::table script;
	public:
		ComponentList createComponentList(sol::variadic_args args)
		{
			ComponentList components;
			for (auto arg : args)
			{
				components.set(arg.get<TypeId>());
			}
			return components;
		}
		ScriptSystem(sol::table script, sol::variadic_args args) : script(script), BaseSystem(createComponentList(args))
		{
		};

		virtual void Process(EntityW::Time deltaTime);
	};
}

#endif