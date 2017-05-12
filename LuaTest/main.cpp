#include <iostream>
#include "sol.hpp"
#include <memory>
#include <map>
#include <windows.h>

typedef int TypeId;

template<typename TBase>
class ClassTypeId {
public:

	template<typename T>
	static TypeId GetTypeId()
	{
		static TypeId id = m_nextTypeId++;
		return id;
	}

	static TypeId GetScriptTypeId() {
		return m_nextTypeId++;
	}
private:

	static TypeId m_nextTypeId;
};

template <typename TBase>
TypeId ClassTypeId<TBase>::m_nextTypeId = 0;

class Test {
public:
	int id;
};

class Derived1 : public Test {
public:
	Derived1(): Test() {
		id = ClassTypeId<Test>::GetScriptTypeId();
	}
};

class Derived2 : public Test {

};

std::map<TypeId, std::shared_ptr<Test>> tests;
/*
std::shared_ptr<Test> createTest(int i)
{
	auto test = std::make_shared<Test>(i);
	tests[i] = test;
	return test;
}

*/

void add(sol::object test)
{
	auto test2 = test.as<Test>();
}

TypeId registerComponent()
{
	return ClassTypeId<Test>::GetScriptTypeId();
}

std::shared_ptr<Test> get(TypeId id)
{
	return tests[id];
}
int main() {

	std::cout << "Derived2: " << ClassTypeId<Test>::GetTypeId<Derived2>() << std::endl;

	sol::state lua;
	lua.open_libraries(sol::lib::base);
	lua.script("print('bark bark bark!')");

	lua.new_usertype<Derived1>("Test",
		sol::constructors<Derived1()>(),
		"id", &Derived1::id,
		sol::base_classes, sol::bases<Test>()
		);
		
	lua["add"] = add;
	lua["get"] = get;
	lua["registerComponent"] = registerComponent;

	//lua["createTest"] = createTest;

	lua.script_file("test.lua");

	return 0;
}