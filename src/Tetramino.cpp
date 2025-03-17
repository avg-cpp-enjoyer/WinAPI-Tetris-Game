#include "Tetramino.h"

void Tetramino::Move(Direction dir) {
	switch (dir) {
	case Direction::DIRECTION_DOWN:
		m_pos.y++;
		break;
	case Direction::DIRECTION_RIGHT:
		m_pos.x++;
		break;
	case Direction::DIRECTION_LEFT:
		m_pos.x--;
		break;
	}
}

void Tetramino::Rotate() {
	m_rotationState = (m_rotationState + 1) % 4;
	m_tetramino = m_rotationStates[m_rotationState];
}

const std::array<vec2, 4>& Tetramino::GetTetramino() const {
	return m_tetramino;
}

vec2 Tetramino::GetPos() const {
	return m_pos;
}

void Tetramino::SetPos(vec2 pos) {
	m_pos = pos;
}

TetraminoType Tetramino::GetType() const {
	return m_type;
}

Tetramino_I::Tetramino_I() : Tetramino(TetraminoType::TETRAMINO_I, vec2{3, -1}) {
	m_tetramino = {{ vec2{0, 1}, vec2{1, 1}, vec2{2, 1}, vec2{3, 1} }};
	m_rotationStates = {{
		{vec2{0, 1}, vec2{1, 1}, vec2{2, 1}, vec2{3, 1}},
		{vec2{1, 0}, vec2{1, 1}, vec2{1, 2}, vec2{1, 3}},
		{vec2{0, 1}, vec2{1, 1}, vec2{2, 1}, vec2{3, 1}},
		{vec2{1, 0}, vec2{1, 1}, vec2{1, 2}, vec2{1, 3}},
	}};
}

Tetramino_J::Tetramino_J() : Tetramino(TetraminoType::TETRAMINO_J, vec2{3, 0}) {
	m_tetramino = {{ vec2{1, 0}, vec2{1, 1}, vec2{1, 2}, vec2{0, 2} }};
	m_rotationStates = {{
		{vec2{1, 0}, vec2{1, 1}, vec2{1, 2}, vec2{0, 2}},
		{vec2{0, 1}, vec2{1, 1}, vec2{2, 1}, vec2{2, 2}},
		{vec2{1, 0}, vec2{2, 0}, vec2{1, 1}, vec2{1, 2}},
		{vec2{0, 0}, vec2{0, 1}, vec2{1, 1}, vec2{2, 1}},
	}};
}

Tetramino_O::Tetramino_O() : Tetramino(TetraminoType::TETRAMINO_O, vec2{3, 0}) {
	m_tetramino = {{ vec2{0, 0}, vec2{0, 1}, vec2{1, 0}, vec2{1, 1} }};
	m_rotationStates = {{
		{vec2{0, 0}, vec2{0, 1}, vec2{1, 0}, vec2{1, 1}},
		{vec2{0, 0}, vec2{0, 1}, vec2{1, 0}, vec2{1, 1}},
		{vec2{0, 0}, vec2{0, 1}, vec2{1, 0}, vec2{1, 1}},
		{vec2{0, 0}, vec2{0, 1}, vec2{1, 0}, vec2{1, 1}},
	}};
}

Tetramino_L::Tetramino_L() : Tetramino(TetraminoType::TETRAMINO_L, vec2{3, 0}) {
	m_tetramino = {{ vec2{1, 0}, vec2{1, 1}, vec2{1, 2}, vec2{2, 2}} };
	m_rotationStates = {{
		{vec2{1, 0}, vec2{1, 1}, vec2{1, 2}, vec2{2, 2}},
		{vec2{0, 1}, vec2{1, 1}, vec2{2, 1}, vec2{2, 0}},
		{vec2{0, 0}, vec2{1, 0}, vec2{1, 1}, vec2{1, 2}},
		{vec2{0, 2}, vec2{0, 1}, vec2{1, 1}, vec2{2, 1}},
	}};
}

Tetramino_Z::Tetramino_Z() : Tetramino(TetraminoType::TETRAMINO_Z, vec2{3, 0}) {
	m_tetramino = {{ vec2{0, 0}, vec2{1, 0}, vec2{1, 1}, vec2{2, 1} }};
	m_rotationStates = {{
		{vec2{0, 0}, vec2{1, 0}, vec2{1, 1}, vec2{2, 1}},
		{vec2{2, 0}, vec2{1, 1}, vec2{2, 1}, vec2{1, 2}},
		{vec2{0, 0}, vec2{1, 0}, vec2{1, 1}, vec2{2, 1}},
		{vec2{2, 0}, vec2{1, 1}, vec2{2, 1}, vec2{1, 2}},
	}};
}

Tetramino_S::Tetramino_S() : Tetramino(TetraminoType::TETRAMINO_S, vec2{3, 0}) {
	m_tetramino = {{ vec2{1, 0}, vec2{2, 0}, vec2{0, 1}, vec2{1, 1} }};
	m_rotationStates = {{
		{vec2{1, 0}, vec2{2, 0}, vec2{0, 1}, vec2{1, 1}},
		{vec2{1, 0}, vec2{1, 1}, vec2{2, 1}, vec2{2, 2}},
		{vec2{1, 0}, vec2{2, 0}, vec2{0, 1}, vec2{1, 1}},
		{vec2{1, 0}, vec2{1, 1}, vec2{2, 1}, vec2{2, 2}},
	}};
}

Tetramino_T::Tetramino_T() : Tetramino(TetraminoType::TETRAMINO_T, vec2{3, -1}) {
	m_tetramino = {{ vec2{0, 1}, vec2{1, 1}, vec2{2, 1}, vec2{1, 2} }};
	m_rotationStates = {{
		{vec2{0, 1}, vec2{1, 1}, vec2{2, 1}, vec2{1, 2}},
		{vec2{1, 0}, vec2{1, 1}, vec2{1, 2}, vec2{0, 1}},
		{vec2{0, 1}, vec2{1, 1}, vec2{2, 1}, vec2{1, 0}},
		{vec2{1, 0}, vec2{1, 1}, vec2{1, 2}, vec2{2, 1}},
	}};
}

