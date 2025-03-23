#pragma once

class vec2 {
public:
	int x = 0;
	int y = 0;

	vec2() = default;
	vec2(int x, int y) : x(x), y(y) {}
	vec2(const vec2& other) : x(other.x), y(other.y) {}

	vec2 operator+(const vec2& other) const {
		return vec2(x + other.x, y + other.y);
	}

	bool operator==(const vec2& other) const {
		return (x == other.x && y == other.y);
	}
};