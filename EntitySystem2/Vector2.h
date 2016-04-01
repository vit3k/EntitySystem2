#pragma once
#include <SFML\System.hpp>
#include <glm.hpp>

/*
template<class T>
class Vector2
{
public:
	T x, y;
	Vector2(T x, T y): x(x), y(y) {};
	Vector2() {};
	Vector2<T>& operator+=(const Vector2<T>& right)
	{
		this->x += right.x;
		this->y += right.y;
		return *this;
	}
	Vector2<T>& operator-=(const Vector2<T>& right)
	{
		this->x -= right.x;
		this->y -= right.y;
		return *this;
	}

	Vector2<T>& operator*(const float right)
	{
		Vector2<T> vector (this->x * right, this->y * right);
		return vector;
	}
};

typedef Vector2<float> Vector2f;
*/

using Vector2 = glm::vec2;
class Vector2Utils
{
public:
	static std::string toString(Vector2 vec)
	{
		return std::to_string(vec.x) + " " + std::to_string(vec.y);
	}
};

/*

class Vector2 : public glm::vec2
{
public:
	Vector2(float x, float y) : glm::vec2(x, y) {}
	Vector2() : glm::vec2() {}
	std::string toString() {
		return std::to_string(x) + " " + std::to_string(y);
	}
};
*/