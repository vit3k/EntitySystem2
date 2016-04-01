#ifndef SYSTEM_H
#define SYSTEM_H

#include "Entity.h"
#include "Component.h"
#include <vector>
#include "Time.h"

namespace EntityW 
{
	class World;

	class BaseSystem 
	{
	protected:
		std::vector<EntitySp> entities;
		World* world;
	public:
		virtual void Process(Time deltaTime);
		virtual void ProcessEntity(EntitySp entity, Time deltaTime);
		virtual void OnEntityAdded(EntitySp entity);
		void setWorld(World* world) { this->world = world; }
		ComponentList components;
		BaseSystem(ComponentList components) : components(components) {};
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
	}
}

#endif