#pragma once
#include "Base.h"
#include "Button.h"
#include "GameField.h"

#include <ShellScalingAPI.h>

#pragma comment(lib, "Shcore.lib")

#define IDC_BUTTON_Q 1003
#define IDC_BUTTON_R 1004

class GameOverWindow : public Base<GameOverWindow> {
public:
	GameOverWindow(int score, int highScore);
	~GameOverWindow();

	PCWSTR ClassName() const override;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	bool ShouldRestart() const;
private:
	void CreateControls();
	void OnPaint();
	float GetScreenDpi() const;

	float m_scaleFactor = 1.0f;
	int m_score;
	int m_highScore;
	bool m_restart = false;
	HBRUSH m_bgBrush;
	std::unique_ptr<Button> m_restartButton;
	std::unique_ptr<Button> m_quitButton;
	HFONT m_hFont;
};
