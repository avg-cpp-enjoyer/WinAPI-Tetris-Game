#pragma once

#include <string>
#include <fstream>
#include <shlobj.h>        
#include <knownfolders.h>  
#include <filesystem>
#include <combaseapi.h>    

class HighScoreManager {
public:
	HighScoreManager(const HighScoreManager&) = delete;
	HighScoreManager& operator=(const HighScoreManager&) = delete;

	static int Get();
	static void CheckAndUpdate(int score);
private:
	explicit HighScoreManager(const std::string& filename);
	~HighScoreManager() = default;

	static HighScoreManager& GetInstance();
	static std::filesystem::path GetFilePath();
	void CheckAndUpdateImpl(int score);
	void Load();
	void Save() const;
private:
	std::string m_filename;
	int m_highScore  = 0;
};

