#include "EventDispatcher.h"

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