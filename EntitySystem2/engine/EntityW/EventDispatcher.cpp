#include "EventDispatcher.h"
#include "../CollisionSystem.h"

namespace EntityW {

	TypeId ScriptEventTypeId()
	{
		return ClassTypeId<BaseEvent>::GetScriptTypeId();
	}

	void EventDispatcher::process()
	{
		// to stage process
		// stage 1 - process events registered in C++
		for (auto e : queues[currentQueue])
		{
			if (listeners.find(e->getTypeId()) != listeners.end())
			{
				for (auto listener : listeners[e->getTypeId()])
				{
					listener(e);
				}
			}

			if (scriptListeners.find(e->getTypeId()) != scriptListeners.end())
			{
				for (auto listener : scriptListeners[e->getTypeId()])
				{
					listener(e);
				}
			}
			if (scriptListenersWithTable.find(e->getTypeId()) != scriptListenersWithTable.end())
			{
				for (auto listener : scriptListenersWithTable[e->getTypeId()])
				{
					if (e->getTypeId() == EventTypeId<MoveUpEvent>())
						listener.second(listener.first, std::static_pointer_cast<MoveUpEvent>(e));

					else if (e->getTypeId() == EventTypeId<MoveDownEvent>())
						listener.second(listener.first, std::static_pointer_cast<MoveDownEvent>(e));

					else if (e->getTypeId() == EventTypeId<LaunchBallEvent>())
						listener.second(listener.first, std::static_pointer_cast<LaunchBallEvent>(e));
				}
			}
            if (dotnetListeners.find(e->getTypeId()) != dotnetListeners.end())
            {
                for (auto listener : dotnetListeners[e->getTypeId()])
                {
                    listener(e);
                }
            }
		}

		queues[currentQueue].clear();

		if (currentQueue == 0)
			currentQueue = 1;
		else
			currentQueue = 0;

		// stage 2 - process events registered by lua

		for (auto e : scriptQueues[currentScriptQueue])
		{

			if (scriptListeners.find(e.first) != scriptListeners.end())
			{
				for (auto listener : scriptListeners[e.first])
				{
					listener(e.second);
				}
			}
			if (scriptListenersWithTable.find(e.first) != scriptListenersWithTable.end())
			{
				for (auto listener : scriptListenersWithTable[e.first])
				{
					listener.second(listener.first, e.second);
				}
			}
		}

		scriptQueues[currentScriptQueue].clear();

		if (currentScriptQueue == 0)
			currentScriptQueue = 1;
		else
			currentScriptQueue = 0;
	}

    void EventDispatcher::subscribe(TypeId eventTypeId, native_engine_event_callback callback)
    {
        if (dotnetListeners.find(eventTypeId) == dotnetListeners.end())
        {
            dotnetListeners[eventTypeId] = std::vector<native_engine_event_callback>();
        }
        dotnetListeners[eventTypeId].push_back(callback);
    }

	void EventDispatcher::scriptSubscribe(TypeId eventTypeId, sol::function listener, sol::table self)
	{
		if (scriptListenersWithTable.find(eventTypeId) == scriptListenersWithTable.end())
		{
			scriptListenersWithTable[eventTypeId] = std::vector<std::pair<sol::table, sol::function>>();
		}
		scriptListenersWithTable[eventTypeId].push_back(std::make_pair(self, listener));
	}

	void EventDispatcher::scriptSubscribe(TypeId eventTypeId, sol::function listener)
	{
		if (scriptListeners.find(eventTypeId) == scriptListeners.end())
		{
			scriptListeners[eventTypeId] = std::vector<sol::function>();
		}
		scriptListeners[eventTypeId].push_back(listener);
	}

	void EventDispatcher::scriptClear()
	{
		scriptListeners.clear();
		scriptListenersWithTable.clear();
	}
	void EventDispatcher::scriptEmit(TypeId eventTypeId, sol::object data)
	{
		//TODO: parse known for C++ side events and push to standard queue

		scriptQueues[currentScriptQueue].push_back(std::make_pair(eventTypeId, data));
	}
}