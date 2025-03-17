#include "TetraminoTypes.h"

#include <array>

struct vec2 {
	vec2 operator+(const vec2& other) const { return { x + other.x, y + other.y }; }

	int x = 0;
	int y = 0;
};

enum class Direction {
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

class Tetramino {
public:
	void Rotate();
	void Move(Direction dir);
	const std::array<vec2, 4>& GetTetramino() const;
	vec2 GetPos() const;
	void SetPos(vec2 pos);
	TetraminoType GetType() const;
protected:
	Tetramino(TetraminoType type, vec2 pos) : m_type(type), m_pos(pos) {}

	std::array<vec2, 4> m_tetramino{};
	std::array<std::array<vec2, 4>, 4> m_rotationStates{};
	int m_rotationState = 0;
	vec2 m_pos;
	TetraminoType m_type;
};

class Tetramino_I : public Tetramino {
public:
	Tetramino_I();
};

class Tetramino_J : public Tetramino {
public:
	Tetramino_J();
};

class Tetramino_O : public Tetramino {
public:
	Tetramino_O();
};

class Tetramino_L : public Tetramino {
public:
	Tetramino_L();
};

class Tetramino_Z : public Tetramino {
public:
	Tetramino_Z();
};

class Tetramino_S : public Tetramino {
public:
	Tetramino_S();
};

class Tetramino_T : public Tetramino {
public:
	Tetramino_T();
};
