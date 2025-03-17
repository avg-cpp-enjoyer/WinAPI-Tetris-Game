#include "HighScoreManager.h"

int HighScoreManager::Get() {
	return GetInstance().m_highScore;
}

void HighScoreManager::CheckAndUpdate(int score) {
	GetInstance().CheckAndUpdateImpl(score);
}

HighScoreManager& HighScoreManager::GetInstance() {
	static HighScoreManager instance("highscore.dat");
	return instance;
}

void HighScoreManager::CheckAndUpdateImpl(int score) {
	if (score > m_highScore) {
		m_highScore = score;
		Save();
	}
}

void HighScoreManager::Load() {
	std::ifstream file(m_filename, std::ios::binary);

	if (!file.is_open()) {
		m_highScore = 0;
		return;
	}

	file.read(reinterpret_cast<char*>(&m_highScore), sizeof(m_highScore));

	if (file.fail() || file.gcount() != sizeof(m_highScore)) {
		m_highScore = 0;
	}

	if (m_highScore < 0) {
		m_highScore = 0;
	}
}

void HighScoreManager::Save() {
	std::ofstream file(m_filename, std::ios::binary);

	if (file.is_open()) {
		file.write(reinterpret_cast<const char*>(&m_highScore), sizeof(m_highScore));
	}
}
