#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include "Component.h"

namespace EntityW {
	class World;

	class Entity {
		std::map<TypeId, ComponentSp> components;
		ComponentList componentList;
		World* world;
	public:
		template <typename T, typename... Args>
		std::shared_ptr<T> attach(Args... args);

		template <typename T>
		std::shared_ptr<T> get();

		bool hasComponents(ComponentList componentList);
		template <typename T>
		bool has() {
			return componentList[ComponentTypeId<T>()];
		}
		long id;
		Entity(World* world, long id) :world(world), id(id) {};
	};

	typedef std::shared_ptr<Entity> EntitySp;

	template <typename T, typename... Args>
	std::shared_ptr<T> Entity::attach(Args... args)
	{
		static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot add T to entity");
		auto component = std::make_shared<T>(std::forward<Args>(args)...);
		components[ComponentTypeId<T>()] = component;
		componentList.set(ComponentTypeId<T>());
		return component;
	}

	template<typename T>
	std::shared_ptr<T> Entity::get()
	{
		static_assert(std::is_base_of<Component, T>(), "T is not a component, cannot get it from entity");
		return std::static_pointer_cast<T>(components[ComponentTypeId<T>()]);
	}


}

#endif