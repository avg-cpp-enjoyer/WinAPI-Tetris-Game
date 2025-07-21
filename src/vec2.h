#pragma once

#include <cmath>

class vec2 {
public:
	float x = 0;
	float y = 0;

	vec2() = default;
	vec2(float x, float y) : x(x), y(y) {}
	vec2(const vec2& other) : x(other.x), y(other.y) {}

	vec2 operator+(const vec2& other) const {
		return { x + other.x, y + other.y };
	}

	vec2 operator-(const vec2& other) const {
		return { x - other.x, y - other.y };
	}

	bool operator==(const vec2& other) const {
		return x == other.x && y == other.y;
	}

	bool operator!=(const vec2& other) const {
		return !(*this == other);
	}

	vec2 operator*(float scalar) const {
		return { x * scalar, y * scalar };
	}

	float Length() const {
		return std::sqrt(x * x + y * y);
	}
};