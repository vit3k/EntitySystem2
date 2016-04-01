#pragma once

#include <vector>
#include "Event.h"
#include <map>
#include "Fastdelegate.h"
#include "Logger.h"

typedef fastdelegate::FastDelegate1<EventSp> EventListenerDelegate;
typedef std::map<EntityW::TypeId, std::vector<EventListenerDelegate>> EventListenersMap;
class EventDispatcher
{
	std::vector<EventSp> queues[2];
	int currentQueue = 0;
	EventListenersMap listeners;
	EventDispatcher() {};
	Logger logger = Logger::get("EventDispatcher");
public:
	static EventDispatcher& get() {
		static EventDispatcher instance;
		return instance;
	};

	template<class T, typename... Args>
	void emit(Args... args) {
		auto e = std::make_shared<T>(std::forward<Args>(args)...);
		queues[currentQueue].push_back(e);
	}

	template<class T, typename... Args>
	void emitNow(Args... args) {
		auto e = std::make_shared<T>(std::forward<Args>(args)...);
		if (listeners.find(e->getTypeId()) != listeners.end())
		{
			for (auto listener : listeners[e->getTypeId()])
			{
				listener(e);
			}
		}
	}

	template<class T>
	void subscribe(EventListenerDelegate listener)
	{
		EntityW::TypeId type = EventTypeId<T>();
		if (listeners.find(type) == listeners.end())
		{
			listeners[type] = std::vector<EventListenerDelegate>();
		}
		listeners[type].push_back(listener);
	}

	
	void process();
};
