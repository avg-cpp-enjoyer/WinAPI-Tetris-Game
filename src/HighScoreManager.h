#pragma once
#include <string>
#include <fstream>

class HighScoreManager {
public:
	HighScoreManager(const HighScoreManager&) = delete;

	static int Get();
	static void CheckAndUpdate(int score);
private:
	HighScoreManager(const std::string& filename)
		: m_filename(filename) {
		Load();
	}

	HighScoreManager() {}
	~HighScoreManager() {}

	static HighScoreManager& GetInstance();
	void CheckAndUpdateImpl(int score);
	void Load();
	void Save();

	std::string m_filename;
	int m_highScore  = 0;
};

