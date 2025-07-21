#include "HighScoreManager.h"

HighScoreManager::HighScoreManager(const std::string& filename) : m_filename(filename) {
	Load();
}

int HighScoreManager::Get() {
	return GetInstance().m_highScore;
}

void HighScoreManager::CheckAndUpdate(int score) {
	GetInstance().CheckAndUpdateImpl(score);
}

HighScoreManager& HighScoreManager::GetInstance() {
	static HighScoreManager instance(GetFilePath().string());
	return instance;
}

std::filesystem::path HighScoreManager::GetFilePath() {
	wchar_t* path = nullptr;
	std::filesystem::path result;

	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path))) {
		result = std::filesystem::path(path) / "Tetris" / "highscore.dat";
		std::filesystem::create_directories(result.parent_path());
	}

	if (path) {
		CoTaskMemFree(path);
	}

	if (!result.empty()) {
		return result;
	}

	return std::filesystem::temp_directory_path() / "Tetris" / "highscore.dat";
}

void HighScoreManager::CheckAndUpdateImpl(int score) {
	if (score > m_highScore) {
		m_highScore = score;
		Save();
	}
}

void HighScoreManager::Load() {
	std::ifstream file(m_filename, std::ios::binary);

	if (!(file && file.read(reinterpret_cast<char*>(&m_highScore), sizeof(m_highScore)))) {
		m_highScore = 0;
	}

	if (m_highScore < 0) {
		m_highScore = 0;
	}
}

void HighScoreManager::Save() const {
	std::ofstream file(m_filename, std::ios::binary);
	if (!file) return;

	file.write(reinterpret_cast<const char*>(&m_highScore), sizeof(m_highScore));
}
