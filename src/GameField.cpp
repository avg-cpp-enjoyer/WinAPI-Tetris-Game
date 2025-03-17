#include "GameField.h"

GameField::GameField() {
	std::random_device rd;
	m_rng.seed(rd());

	m_nextTetramino = CreateRandomTetramino();
	SpawnNewTetramino();
}

bool GameField::MoveCurrent(Direction dir) {
	vec2 originalPos = m_currentTetramino->GetPos();
	m_currentTetramino->Move(dir);

	if (!TetraminoFits(*m_currentTetramino)) {
		m_currentTetramino->SetPos(originalPos);
		return false;
	}

	return true;
}

bool GameField::RotateCurrent() {
	m_currentTetramino->Rotate();

	vec2 originalPos = m_currentTetramino->GetPos();
	std::array<vec2, 7> offsets = { vec2{0, 0}, vec2{-1, 0}, vec2{1, 0}, vec2{-2, 0}, vec2{2, 0}, vec2{0, 1}, vec2{0, 2} };

	for (const auto& offset : offsets) {
		m_currentTetramino->SetPos(originalPos + offset);
		if (TetraminoFits(*m_currentTetramino)) {
			return true;
		}
	}

	m_currentTetramino->SetPos(originalPos);

	m_currentTetramino->Rotate();
	m_currentTetramino->Rotate();
	m_currentTetramino->Rotate();

	return false;
}

std::unique_ptr<Tetramino> GameField::CreateRandomTetramino() {
	switch (m_distribution(m_rng)) {
	case 0: return std::make_unique<Tetramino_I>();
	case 1: return std::make_unique<Tetramino_J>();
	case 2: return std::make_unique<Tetramino_L>();
	case 3: return std::make_unique<Tetramino_O>();
	case 4: return std::make_unique<Tetramino_S>();
	case 5: return std::make_unique<Tetramino_Z>();
	case 6: return std::make_unique<Tetramino_T>();
	}

	return nullptr;
}

void GameField::HardDrop() {
	while (MoveCurrent(Direction::DIRECTION_DOWN)) {}
	LockTetramino();
}

void GameField::Update() {
	if (!MoveCurrent(Direction::DIRECTION_DOWN)) {
		LockTetramino();
	}
}

void GameField::Pause() {
	m_isPaused = !m_isPaused;
}

void GameField::Reset(){
	for (int i = 0; i < GameField::WIDTH; i++) {
		for (int j = 0; j < GameField::HEIGHT; j++) {
			m_grid[i][j] = TetraminoType::TETRAMINO_NONE;
		}
	}

	m_score = 0;
	m_gameOver = false;
	m_isPaused = false;

	m_nextTetramino = CreateRandomTetramino();
	SpawnNewTetramino();
}

bool GameField::IsPaused() const {
	return m_isPaused;
}

bool GameField::IsGameOver() const {
	return m_gameOver;
}

int GameField::GetScore() const {
	return m_score;
}

int GameField::GetHighScore() const {
  	return HighScoreManager::Get();
}

const std::array<std::array<TetraminoType, GameField::HEIGHT>, GameField::WIDTH>& GameField::GetGrid() const {
	return m_grid;
}

const Tetramino* GameField::GetCurrentTetramino() const {
	return m_currentTetramino.get();
}

const Tetramino* GameField::GetNextTetramino() const {
	return m_nextTetramino.get();
}

bool GameField::TetraminoFits(const Tetramino& tetramino) const {
	for (const auto& block : tetramino.GetTetramino()) {
		const vec2 pos = tetramino.GetPos() + block;

		if (pos.y < 0 || pos.y >= HEIGHT) {
			return false;
		}

		if (pos.x < 0 || pos.x >= WIDTH) {
			return false;
		}

		if (pos.y >= 0 && m_grid[pos.x][pos.y] != TetraminoType::TETRAMINO_NONE) {
			return false;
		}
	}

	return true;
}

void GameField::LockTetramino() {
	for (const auto& block : m_currentTetramino->GetTetramino()) {
		const vec2 pos = m_currentTetramino->GetPos() + block;
		if (pos.x >= 0 && pos.x < WIDTH && pos.y >= 0 && pos.y < HEIGHT) {
			m_grid[pos.x][pos.y] = m_currentTetramino->GetType();
		}
	}

	ClearLines();
	SpawnNewTetramino();

	if (!TetraminoFits(*m_currentTetramino)) {
		m_gameOver = true;
		HighScoreManager::CheckAndUpdate(m_score);
	}
}

void GameField::ClearLines() {
	int newY = HEIGHT - 1;
	int linesCleared = 0;

	for (int y = HEIGHT - 1; y >= 0; y--) {
		bool lineFull = true;
		for (int x = 0; x < WIDTH; x++) {
			if (m_grid[x][y] == TetraminoType::TETRAMINO_NONE) {
				lineFull = false;
				break;
			}
		}

		if (!lineFull) {
			for (int x = 0; x < WIDTH; x++) {
				m_grid[x][newY] = m_grid[x][y];
			}
			newY--;
		}

		if (lineFull) {
			linesCleared++;
		}

	}

	while (newY >= 0) {
		for (int x = 0; x < WIDTH; x++) {
			m_grid[x][newY] = TetraminoType::TETRAMINO_NONE;
		}
		newY--;
	}

	AddScore(linesCleared);
}

void GameField::AddScore(int lines) {
	constexpr int SCORE_TABLE[] = { 0, 100, 300, 500, 800 };
	m_score += SCORE_TABLE[lines];
}

void GameField::SpawnNewTetramino() {
	m_currentTetramino = std::move(m_nextTetramino);
	m_nextTetramino = CreateRandomTetramino();
}
