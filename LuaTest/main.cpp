#include <iostream>
#include "sol.hpp"
#include <memory>
#include <map>
#include <windows.h>

class Base {
public:
	std::string test;
	Base(std::string test) : test(test) {}
};

class Derived1 : public Base {
public:
	Derived1() : Base("derived1") {}
};

class Derived2 : public Base {
public:
	std::shared_ptr<Derived1> derived1;
	Derived2(std::shared_ptr<Derived1> derived1) : derived1(derived1), Base("derived2") {}
};

void func(sol::object obj)
{
	if (obj.is<Base>())
	{
		auto base = obj.as<Base>();
		std::cout << base.test << std::endl;
	}
}
int main() {


	sol::state lua;

	lua.open_libraries(sol::lib::base);

	lua.new_usertype<Base>("Base",
		sol::constructors<Base(std::string)>()
		);

	lua.new_usertype<Derived1>("Derived1",
		sol::base_classes, sol::bases<Base>());

	lua.new_usertype<Derived2>("Derived2",
		sol::constructors<Derived2(std::shared_ptr<Derived1>)>(),
		sol::base_classes, sol::bases<Base>());

	lua["func"] = func;
	lua.script_file("test.lua");

	return 0;
}