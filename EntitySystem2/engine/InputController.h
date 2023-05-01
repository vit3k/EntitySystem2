#pragma once

class InputController
{
	int controller;
public:
	InputController(int controller) : controller(controller) {};
	void process();
};