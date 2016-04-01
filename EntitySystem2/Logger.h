#pragma once

#include <string>
#include <iostream>

class Logger
{
	std::string className;
	Logger(std::string className) : className(className) {};
public:
	static Logger get(std::string className) {
		return Logger(className);
	}
	void log(std::string msg)
	{
		std::cout << className << ": " << msg << std::endl;
	}
};
