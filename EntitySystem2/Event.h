#pragma once

#include "EntityW\EventDispatcher.h"

template<class T>
class InputEvent : public EntityW::Event<T>
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

class LaunchBallEvent : public InputEvent<LaunchBallEvent> {
public:
	LaunchBallEvent(int controller) : InputEvent(controller) {};
};


