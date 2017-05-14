#include <iostream>
#include "sol.hpp"
#include <memory>

class SuperData{
	
public:
	int test = 0;
	~SuperData()
	{
		std::cout << "SuperData destroyed" << std::endl;
	}
};

class Component{
public:
	std::shared_ptr<SuperData> superData;
	SuperData superData2;
	Component(std::shared_ptr<SuperData> superData, SuperData superData2) : superData(superData), superData2(superData2) {}
};

std::shared_ptr<Component> component;

std::shared_ptr<Component> create(SuperData superData, SuperData superData2)
{
	component = std::make_shared<Component>(std::shared_ptr<SuperData>(&superData), superData2);
	return component;
}

int main() {
	sol::state lua;
	lua.open_libraries(sol::lib::base);

	lua.new_usertype<SuperData>("SuperData",
		"test", &SuperData::test
	);

	lua.new_usertype<Component>("Component",
		sol::constructors<Component(std::shared_ptr<SuperData>, SuperData superData2)>(),
		"superData", &Component::superData,
		"superData2", &Component::superData2
	);

	lua["create"] = create;
	lua.script_file("test.lua");
	
	while (1)
	{
		lua["test"](component);
		std::cout << "C++: " << component->superData->test << " " << component->superData2.test << std::endl;
	}
	return 0;
}