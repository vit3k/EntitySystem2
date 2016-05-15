#pragma once
#include <SFML\System.hpp>
#include <glm.hpp>

using Vector2 = glm::vec2;

class Vector2Utils
{
public:
	static std::string toString(Vector2 vec)
	{
		return std::to_string(vec.x) + " " + std::to_string(vec.y);
	}
};
