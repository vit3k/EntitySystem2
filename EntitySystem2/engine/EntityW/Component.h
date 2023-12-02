#ifndef COMPONENT_H
#define COMPONENT_H

#include "ClassTypeId.h"
#include <bitset>
#include <memory>
#include <string>

namespace EntityW {
	class BaseComponent
	{
	public:
		[[nodiscard]] virtual TypeId getTypeId() const = 0;
        virtual ~BaseComponent() = default;
	};

	template <class T>
	TypeId ComponentTypeId();

	template<typename T>
	class Component : public BaseComponent {
	public:
		[[nodiscard]] TypeId getTypeId() const override {
			return ComponentTypeId<T>();
		}
	};

	template <class T>
	TypeId ComponentTypeId()
	{
		return ClassTypeId<BaseComponent>::GetTypeId<T>();
	}

	TypeId ScriptComponentTypeId();

	using ComponentList = std::bitset<32>;

	typedef std::shared_ptr<BaseComponent> ComponentSp;
}

#endif