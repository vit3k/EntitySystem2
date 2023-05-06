#pragma once

#include "EntityW/Time.h"

class IScriptManager {
	public:
	virtual void init() = 0;
	virtual void process(EntityW::Time deltaTime) = 0;
    virtual void close() = 0;
};