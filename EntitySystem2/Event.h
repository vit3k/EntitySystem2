#pragma once

#include <memory>
#include "EntityW\ClassTypeId.h"
#include <string>

class BaseEvent
{
public:
	virtual EntityW::TypeId getTypeId() = 0;
	virtual std::string toString()
	{
		return std::to_string(getTypeId());
	}
};

template<class T>
class Event : public BaseEvent
{
public:
	virtual EntityW::TypeId getTypeId() {
		return EventTypeId<T>();
	};
};

template <class T>
EntityW::TypeId EventTypeId()
{
	return EntityW::ClassTypeId<BaseEvent>::GetTypeId<T>();
}

template<class T>
class InputEvent : public Event<T>
{
public:
	int controller;
	InputEvent(int controller) : controller(controller) {};
};
class MoveUpEvent : public InputEvent<MoveUpEvent>
{
public:
	MoveUpEvent(int controller) : InputEvent(controller) {};
};
class MoveDownEvent : public InputEvent<MoveDownEvent>
{
public:
	MoveDownEvent(int controller) : InputEvent(controller) {};
};
class StopEvent : public InputEvent<StopEvent>
{
public:
	StopEvent(int controller) : InputEvent(controller) {};
};

typedef std::shared_ptr<BaseEvent> EventSp;