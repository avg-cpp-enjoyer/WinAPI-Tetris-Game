#include "GameField.h"

GameField::GameField() {
	UpdateGhostPos();
}

bool GameField::MoveCurrent(Direction dir) {
	const vec2& originalPos = m_currentTetramino.GetPos();
	m_currentTetramino.Move(dir);

	if (!TetraminoFits(m_currentTetramino)) {
		m_currentTetramino.SetPos(originalPos);
		return false;
	}

	return true;
}

bool GameField::RotateCurrent() {
	m_currentTetramino.Rotate();

	const vec2& originalPos = m_currentTetramino.GetPos();
	static const std::array<vec2, 7> offsets = { vec2(0.0f, 0.0f), vec2(-1.0f, 0.0f), vec2(1.0f, 0.0f), 
		vec2(-2.0f, 0.0f), vec2(2.0f, 0.0f), vec2(0.0f, 1.0f), vec2(0.0f, 2.0f) 
	};

	for (const auto& offset : offsets) {
		m_currentTetramino.SetPos(originalPos + offset);
		if (TetraminoFits(m_currentTetramino)) {
			m_ghostTetramino.Rotate();
			return true;
		}
	}

	m_currentTetramino.SetPos(originalPos);
	m_currentTetramino.UndoRotation();

	return false;
}

void GameField::HardDrop() {
	m_currentTetramino.HardDrop(m_ghostTetramino.GetPos());
}

void GameField::Update() {
	UpdateGhostPos();

	if (!MoveCurrent(Direction::DIRECTION_DOWN)) {
		LockTetramino();
	}
}

void GameField::Reset(){
	for (auto& row : m_grid) {
		row.fill(TetraminoType::TETRAMINO_NONE);
	}

	m_score = 0;
	m_gameOver = false;

	m_nextTetramino = TetraminoManager::CreateRandomTetramino();
	SpawnNewTetramino();
}

bool GameField::IsGhostCollide() const {
	const vec2& ghostPos = m_ghostTetramino.GetPos();
	const vec2& currentPos = m_currentTetramino.GetPos();

	for (const auto& ghostBlock : m_ghostTetramino.GetTetramino()) {
		for (const auto& currentBlock : m_currentTetramino.GetTetramino()) {
			if (currentBlock + currentPos == ghostBlock + ghostPos) {
				return true;
			}
		}
	}

	return false;
}

bool GameField::IsGameOver() const {
	return m_gameOver;
}

int GameField::GetScore() const {
	return m_score;
}

int GameField::GetHighScore() {
  	return HighScoreManager::Get();
}

const std::array<std::array<TetraminoType, gfHeight>, gfWidth>& GameField::GetGrid() const {
	return m_grid;
}

const Tetramino& GameField::GetCurrentTetramino() const {
	return m_currentTetramino;
}

const Tetramino& GameField::GetGhostTetramino() const {
	return m_ghostTetramino;
}

const Tetramino& GameField::GetNextTetramino() const {
	return m_nextTetramino;
}

Tetramino& GameField::GetCurrentTetramino() {
	return m_currentTetramino;
}

Tetramino& GameField::GetGhostTetramino() {
	return m_ghostTetramino;
}

void GameField::ClearLinesAndSpawn() {
	LockTetramino();
}

bool GameField::TetraminoFits(const Tetramino& tetramino) const {
	for (const auto& block : tetramino.GetTetramino()) {
		const vec2& pos = tetramino.GetPos() + block;

		if (pos.y < 0 || pos.y >= UI::MainWindow::GameField::gfHeight) {
			return false;
		}

		if (pos.x < 0 || pos.x >= UI::MainWindow::GameField::gfWidth) {
			return false;
		}

		if (pos.y >= 0 && m_grid[static_cast<int>(pos.x)][static_cast<int>(pos.y)] != TetraminoType::TETRAMINO_NONE) {
			return false;
		}
	}

	return true;
}

void GameField::LockTetramino() {
	m_currentTetramino.SetPos(m_ghostTetramino.GetPos());

	for (const auto& block : m_currentTetramino.GetTetramino()) {
		const vec2& pos = m_currentTetramino.GetPos() + block;
		if (pos.x >= 0 && pos.x < gfWidth && pos.y >= 0 && pos.y < gfHeight) {
			m_grid[static_cast<int>(pos.x)][static_cast<int>(pos.y)] = m_currentTetramino.GetType();
		}
	}

	ClearLines();

	if (!TetraminoFits(m_nextTetramino)) {
		m_gameOver = true;
		HighScoreManager::CheckAndUpdate(m_score);
		return;
	}

	SpawnNewTetramino();
}

void GameField::UpdateGhostPos() {
	m_ghostTetramino = m_currentTetramino;

	while (true) {
		m_ghostTetramino.Move(Direction::DIRECTION_DOWN);
		if (!TetraminoFits(m_ghostTetramino)) {
			m_ghostTetramino.SetPos(vec2(m_ghostTetramino.GetPos().x, m_ghostTetramino.GetPos().y - 1));
			break;
		}
	}
}

void GameField::ClearLines() {
	int writeY = gfHeight - 1;
	int linesCleared = 0;

	for (int y = gfHeight - 1; y >= 0; y--) {
		bool full = true;
		for (int x = 0; x < gfWidth; ++x) {
			if (m_grid[x][y] == TetraminoType::TETRAMINO_NONE) {
				full = false;
				break;
			}
		}

		if (!full) {
			if (y != writeY) {
				for (int x = 0; x < gfWidth; x++) {
					m_grid[x][writeY] = m_grid[x][y];
				}
			}
			writeY--;
		} else {
			linesCleared++;
		}
	}

	for (int y = writeY; y >= 0; y--) {
		for (int x = 0; x < gfWidth; x++) {
			m_grid[x][y] = TetraminoType::TETRAMINO_NONE;
		}
	}

	AddScore(linesCleared);
}

void GameField::AddScore(int lines) {
	static constexpr int SCORE_TABLE[] = { 0, 100, 300, 500, 800 };
	m_score += SCORE_TABLE[lines];
}

void GameField::SpawnNewTetramino() {
	m_currentTetramino = m_nextTetramino;
	m_ghostTetramino = TetraminoManager::CreateTetramino(m_currentTetramino.GetType());
	m_nextTetramino = TetraminoManager::CreateRandomTetramino();
	UpdateGhostPos();
}
