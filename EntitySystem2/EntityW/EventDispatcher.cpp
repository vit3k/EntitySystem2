#include "EventDispatcher.h"
#include "../CollisionSystem.h"

namespace EntityW {
	void EventDispatcher::process()
	{
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
		}

		queues[currentQueue].clear();

		if (currentQueue == 0)
			currentQueue = 1;
		else
			currentQueue = 0;
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
}