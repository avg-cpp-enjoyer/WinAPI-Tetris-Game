#pragma once

#include <array>

#include "Tetramino.h"
#include "TetraminoManager.h"
#include "HighScoreManager.h"
#include "Constants.h"

class GameField {
public:
	GameField();
	~GameField() = default;

	bool MoveCurrent(Direction dir);
	bool RotateCurrent();
	void UpdateGhostPos();
	void HardDrop();
	void Update();
	void Reset();
	bool IsGhostCollide() const;
	bool IsGameOver() const;
	int GetScore() const;
	static int GetHighScore();
	const std::array<std::array<TetraminoType, Constants::gameFieldHeight>, Constants::gameFieldWidth>& GetGrid() const;
	const Tetramino& GetCurrentTetramino() const;
	const Tetramino& GetGhostTetramino() const;
	const Tetramino& GetNextTetramino() const;
	Tetramino& GetCurrentTetramino();
	Tetramino& GetGhostTetramino();
	void ClearLinesAndSpawn();
	void SpawnNewTetramino();
	bool TetraminoFits(const Tetramino& tetramino) const;
	void LockTetramino();
	void ClearLines();
private:
	void AddScore(int lines);
private:
	std::array<std::array<TetraminoType, Constants::gameFieldHeight>, Constants::gameFieldWidth> m_grid{};
	Tetramino m_currentTetramino = TetraminoManager::CreateRandomTetramino();
	Tetramino m_nextTetramino = TetraminoManager::CreateRandomTetramino();
	Tetramino m_ghostTetramino = TetraminoManager::CreateTetramino(m_currentTetramino.GetType());
	int m_score = 0;
	bool m_gameOver = false;
};