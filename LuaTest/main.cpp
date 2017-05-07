#include <iostream>
#include "sol.hpp"
#include <memory>
#include <map>
#include <windows.h>

class Test {
public:
	Test(int i) : i(i) {};
	int i;
};

std::map<int, std::shared_ptr<Test>> tests;

std::shared_ptr<Test> createTest(int i)
{
	auto test = std::make_shared<Test>(i);
	tests[i] = test;
	return test;
}

int main() {
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	lua.script("print('bark bark bark!')");

	lua.new_usertype<Test>("Test",
		sol::constructors<Test(int)>(),
		"i", &Test::i
		);

	lua["createTest"] = createTest;

	lua.script_file("test.lua");
	lua.collect_garbage();
	Sleep(5000);
	std::cout << tests[5]->i << std::endl;

	return 0;
}