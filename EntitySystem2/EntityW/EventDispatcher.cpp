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
		}

		queues[currentQueue].clear();

		if (currentQueue == 0)
			currentQueue = 1;
		else
			currentQueue = 0;
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