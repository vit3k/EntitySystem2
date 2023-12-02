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
		virtual void OnEntityRemoved(EventSp event);
        virtual void EntityAddedCallback(EntitySp entity) {};
		//void setWorld(World* world) { this->world = world; }
		ComponentList components;
		BaseSystem(ComponentList components) : components(components), logger(Logger::get(getName()))
		{
			EventDispatcher::get().subscribe<EntityCreatedEvent>(EntityW::EventListenerDelegate(this, &BaseSystem::OnEntityAdded));
			EventDispatcher::get().subscribe<ComponentAttachedEvent>(EntityW::EventListenerDelegate(this, &BaseSystem::OnComponentAttached));
			EventDispatcher::get().subscribe<EntityRemovedEvent>(EntityW::EventListenerDelegate(this, &BaseSystem::OnEntityRemoved));
		};
		BaseSystem() : logger(Logger::get(getName()))
		{
			EventDispatcher::get().subscribe<EntityCreatedEvent>(EntityW::EventListenerDelegate(this, &BaseSystem::OnEntityAdded));
			EventDispatcher::get().subscribe<ComponentAttachedEvent>(EntityW::EventListenerDelegate(this, &BaseSystem::OnComponentAttached));
			EventDispatcher::get().subscribe<EntityRemovedEvent>(EntityW::EventListenerDelegate(this, &BaseSystem::OnEntityRemoved));
		};
	};

	template<class... Args>
	struct TypeList {};

	template<class... Args>
	class System : public BaseSystem
	{
	public:


		template <class... Args2>
		ComponentList createComponentList(TypeList<Args2...> typeList)
		{
			return ComponentList();
		};

		template <class T, class... Args2>
		ComponentList createComponentList(TypeList<T, Args2...> typeList)
		{
			static_assert(std::is_base_of<Component<T>, T>::value, "Invalid component");
			auto test = ComponentTypeId<T>();
			return ComponentList().set(test) | createComponentList(TypeList<Args2...>());
		};

		System() : BaseSystem(createComponentList(TypeList<Args...>{})) {
		};
	};

	typedef std::shared_ptr<BaseSystem> SystemSp;

	template <class T>
	TypeId SystemTypeId()
	{
		return ClassTypeId<System<>>::GetTypeId<T>();
	};

	class ScriptSystem : public BaseSystem
	{
		sol::table script;
	public:
		void createComponentList(sol::table args)
		{
			args.for_each([this](sol::object key, sol::object value)
			{
				logger.log("adding component to system list : " + std::to_string(value.as<TypeId>()));
				components.set(value.as<TypeId>());
			});
		}
		virtual void OnScriptComponentAttached(EventSp event);
		virtual void OnScriptComponentDetached(EventSp event);
		ScriptSystem(sol::table script) : script(script)
		{
			EventDispatcher::get().subscribe<ScriptComponentAttachedEvent>(EntityW::EventListenerDelegate(this, &ScriptSystem::OnScriptComponentAttached));
			EventDispatcher::get().subscribe<ScriptComponentDetachedEvent>(EntityW::EventListenerDelegate(this, &ScriptSystem::OnScriptComponentDetached));
			createComponentList(script["requiredComponents"]);
			if (script["init"].valid())
			{
				script["init"](script);
			}
		};
		virtual std::string getName() { return script["name"];  }
		virtual void Process(EntityW::Time deltaTime);

	};
}

#endif