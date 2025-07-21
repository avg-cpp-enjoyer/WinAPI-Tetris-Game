#include "Tetramino.h"

Tetramino::Tetramino(TetraminoType type, const vec2& pos) : m_pos(pos), m_type(type), m_visualPos(pos), m_targetPos(pos) {}

TetraminoType Tetramino::GetType() const {
	return m_type;
}

const std::array<vec2, 4>& Tetramino::GetTetramino() const {
	return m_tetramino;
}

vec2 Tetramino::GetPos() const {
	return m_pos;
}

vec2 Tetramino::GetVisualPos() const {
	return m_visualPos;
}

void Tetramino::SetPos(const vec2& pos) {
	m_pos = pos;
	m_targetPos = pos;
	m_startPos = pos;
	m_visualPos = pos;
}

bool Tetramino::IsDropping() const {
	return m_isDropping;
}

void Tetramino::HardDrop(const vec2& ghostPos) {
	m_isDropping = true;
	m_startPos = m_visualPos;
	m_targetPos = ghostPos;
	m_moveElapsed = 0.0f;
	m_moveDuration = std::abs(m_targetPos.y - m_startPos.y) / m_dropSpeed;
}

void Tetramino::Move(Direction dir) {
	m_startPos = m_visualPos;
	m_moveElapsed = 0.0f;

	switch (dir) {
	case Direction::DIRECTION_DOWN:  
		m_pos.y += 1.0f; break;
	case Direction::DIRECTION_LEFT:  
		m_pos.x -= 1.0f; break;
	case Direction::DIRECTION_RIGHT: 
		m_pos.x += 1.0f; break;
	}

	m_targetPos = m_pos;

	float dx = m_targetPos.x - m_startPos.x;
	float dy = m_targetPos.y - m_startPos.y;
	m_moveDuration = std::sqrt(dx * dx + dy * dy) / m_animSpeed;
}

void Tetramino::Rotate() {
	m_rotationState = (m_rotationState + 1) % 4;
	m_tetramino = m_rotationStates[m_rotationState];
}

void Tetramino::UndoRotation() {
	m_rotationState = (m_rotationState + 3) % 4;
	m_tetramino = m_rotationStates[m_rotationState];
}

bool Tetramino::IsAnimating() const {
	return m_moveElapsed < m_moveDuration;
}

void Tetramino::UpdateAnimation(float deltaTime) {
	if (m_moveDuration <= 0) return;

	m_moveElapsed += deltaTime;
	if (m_moveElapsed >= m_moveDuration) {
		m_visualPos = m_targetPos;
		m_moveDuration = 0;
		return;
	}

	float t = m_moveElapsed / m_moveDuration;
	float ease = 1.0f - std::pow(1.0f - t, 3.0f);

	m_visualPos.x = m_startPos.x + (m_targetPos.x - m_startPos.x) * ease;
	m_visualPos.y = m_startPos.y + (m_targetPos.y - m_startPos.y) * ease;

	if (t >= 1.0f) {
		m_visualPos = m_targetPos;
		m_moveDuration = 0.0f;
		m_isDropping ? m_isDropping = false : true;
	}
}

Tetramino_I::Tetramino_I() : Tetramino(TetraminoType::TETRAMINO_I, vec2(3.0f, -1.0f)) {
	m_tetramino = {{{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 3.0f, 1.0f }}};
	m_rotationStates = {{
		{{{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 3.0f, 1.0f }}},
		{{{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }, { 1.0f, 3.0f }}},
		{{{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 3.0f, 1.0f }}},
		{{{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }, { 1.0f, 3.0f }}},
	}};
}

Tetramino_J::Tetramino_J() : Tetramino(TetraminoType::TETRAMINO_J, vec2(4.0f, 0.0f))
{
	m_tetramino = {{{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }, { 0.0f, 2.0f }}};
	m_rotationStates = {{
		{{{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }, { 0.0f, 2.0f }}},
		{{{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 2.0f, 2.0f }}},
		{{{ 1.0f, 0.0f }, { 2.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }}},
		{{{ 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }}},
	}};
}

Tetramino_O::Tetramino_O() : Tetramino(TetraminoType::TETRAMINO_O, vec2(4.0f, 0.0f)) {
	m_tetramino = {{{ 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }}};
	m_rotationStates = {{
		{{{ 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }}},
		{{{ 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }}},
		{{{ 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }}},
		{{{ 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }}},
	}};
}

Tetramino_L::Tetramino_L() : Tetramino(TetraminoType::TETRAMINO_L, vec2(3.0f, 0.0f)) {
	m_tetramino = {{{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f }}};
	m_rotationStates = {{
		{{{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f }}},
		{{{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 2.0f, 0.0f }}},
		{{{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }}},
		{{{ 0.0f, 2.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }}},
	}};
}

Tetramino_S::Tetramino_S() : Tetramino(TetraminoType::TETRAMINO_S, vec2(3.0f, 0.0f)) {
	m_tetramino = {{{ 1.0f, 0.0f}, { 2.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }}};
	m_rotationStates = {{
		{{{ 1.0f, 0.0f }, { 2.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }}},
		{{{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 2.0f, 2.0f }}},
		{{{ 1.0f, 0.0f }, { 2.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }}},
		{{{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 2.0f, 2.0f }}},
	}};
}

Tetramino_Z::Tetramino_Z() : Tetramino(TetraminoType::TETRAMINO_Z, vec2(3.0f, 0.0f)) {
	m_tetramino = {{{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }}};
	m_rotationStates = {{
		{{{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }}},
		{{{ 2.0f, 0.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 1.0f, 2.0f }}},
		{{{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }}},
		{{{ 2.0f, 0.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 1.0f, 2.0f }}},
	}};
}

Tetramino_T::Tetramino_T() : Tetramino(TetraminoType::TETRAMINO_T, vec2(3.0f, -1.0f)) {
	m_tetramino = {{{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 1.0f, 2.0f }}};
	m_rotationStates = {{
		{{{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 1.0f, 2.0f }}},
		{{{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }, { 0.0f, 1.0f }}},
		{{{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f }, { 1.0f, 0.0f }}},
		{{{ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 2.0f }, { 2.0f, 1.0f }}},
	}};
}