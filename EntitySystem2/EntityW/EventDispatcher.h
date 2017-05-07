#pragma once

#include <vector>
#include <map>
#include "Fastdelegate.h"
//#include "Logger.h"
#include "ClassTypeId.h"
#include <memory>
#include <string>

namespace EntityW {
	class BaseEvent
	{
	public:
		virtual TypeId getTypeId() = 0;
		virtual std::string toString()
		{
			return std::to_string(getTypeId());
		}
	};

	template<class T>
	class Event : public BaseEvent
	{
	public:
		virtual TypeId getTypeId() {
			return EventTypeId<T>();
		};
	};

	template <class T>
	TypeId EventTypeId()
	{
		return ClassTypeId<BaseEvent>::GetTypeId<T>();
	}

	typedef std::shared_ptr<BaseEvent> EventSp;

	typedef fastdelegate::FastDelegate1<EventSp> EventListenerDelegate;
	typedef std::map<TypeId, std::vector<EventListenerDelegate>> EventListenersMap;
	class EventDispatcher
	{
		std::vector<EventSp> queues[2];
		int currentQueue = 0;
		EventListenersMap listeners;
		EventDispatcher() {};
//		Logger logger = Logger::get("EventDispatcher");
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
			TypeId type = EventTypeId<T>();
			if (listeners.find(type) == listeners.end())
			{
				listeners[type] = std::vector<EventListenerDelegate>();
			}
			listeners[type].push_back(listener);
		}


		void process();
	};


}