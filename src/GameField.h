#pragma once

#include <array>
#include <memory>
#include <random>
#include <fstream>

#include "Tetramino.h"
#include "HighScoreManager.h"

class GameField {
public:
	static constexpr int WIDTH = 10;
	static constexpr int HEIGHT = 20;

	GameField();

	bool MoveCurrent(Direction dir);
	bool RotateCurrent();
	void HardDrop();
	void Update();
	void Pause();
	void Reset();

	bool IsPaused() const;
	bool IsGameOver() const;
	int GetScore() const;
	int GetHighScore() const;

	const std::array<std::array<TetraminoType, HEIGHT>, WIDTH>& GetGrid() const;
	const Tetramino* GetCurrentTetramino() const;
	const Tetramino* GetNextTetramino() const;
private:
	std::unique_ptr<Tetramino> CreateRandomTetramino();
	void SpawnNewTetramino();
	bool TetraminoFits(const Tetramino& tetramino) const;
	void LockTetramino();
	void ClearLines();
	void AddScore(int lines);

	std::array<std::array<TetraminoType, HEIGHT>, WIDTH> m_grid{};

	std::unique_ptr<Tetramino> m_currentTetramino;
	std::unique_ptr<Tetramino> m_nextTetramino;

	std::mt19937 m_rng;
	std::uniform_int_distribution<int> m_distribution { 0, 6 };

	int m_score = 0;
	bool m_gameOver = false;
	bool m_isPaused = false;
};