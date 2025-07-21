#pragma once

#include "TetraminoTypes.h"
#include "vec2.h"
#include <array>

enum class Direction {
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

class Tetramino {
public:
	void Rotate();
	void UndoRotation();
	void Move(Direction dir);
	const std::array<vec2, 4>& GetTetramino() const;
	TetraminoType GetType() const;
	vec2 GetPos() const;
	void SetPos(const vec2& pos);
	bool IsDropping() const;
	void HardDrop(const vec2& ghostPos);
	void UpdateAnimation(float deltaTime);
	bool IsAnimating() const;
	vec2 GetVisualPos() const;
protected:
	Tetramino(TetraminoType type, const vec2& pos);

	std::array<vec2, 4> m_tetramino;
	std::array<std::array<vec2, 4>, 4> m_rotationStates;
	int m_rotationState = 0;
	vec2 m_pos;
	TetraminoType m_type;

	vec2   m_visualPos;      
	vec2   m_targetPos;    
	vec2   m_startPos;     
	float  m_moveElapsed = 0.f;
	float  m_moveDuration = 0.f;
	float  m_animSpeed = 8.0f;
	float  m_dropSpeed = 20.0f;
	bool   m_isDropping = false;
};

#define DEFINE_TETRAMINO(name) class name : public Tetramino { public: name(); };

DEFINE_TETRAMINO(Tetramino_I)
DEFINE_TETRAMINO(Tetramino_J)
DEFINE_TETRAMINO(Tetramino_O)
DEFINE_TETRAMINO(Tetramino_L)
DEFINE_TETRAMINO(Tetramino_S)
DEFINE_TETRAMINO(Tetramino_Z)
DEFINE_TETRAMINO(Tetramino_T)

#undef DEFINE_TETRAMINO