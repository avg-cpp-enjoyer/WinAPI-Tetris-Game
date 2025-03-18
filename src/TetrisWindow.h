#pragma once

#include "GameField.h"
#include "ResourceManager.h"
#include "GameOverWindow.h"
#include "Base.h"
#include "Button.h"

#include <unordered_map>
#include <gdiplus.h>
#include <Shlwapi.h> 
#include <ShellScalingAPI.h>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Shcore.lib")

#define IDC_BUTTON_1 1001
#define IDC_BUTTON_2 1002

class TetrisWindow : public Base<TetrisWindow> {
public:
	int windowWidth;
	int windowHeight;

	TetrisWindow();
	~TetrisWindow();

	PCWSTR ClassName() const override;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
private:
	void OnPaint();
	void OnKeyDown(WPARAM key);
	void UpdateGame();
	void UpdateWnd() const;
	void DrawRoundedText(HDC hdc, RECT rect, const std::wstring& text);
	void DrawNextTetraminoWnd(HDC hdc, RECT rect);
	void AddButtons();
	void CreateGameOverWindow();
	void PauseGame();
	void CreateBackBuffer(int width, int height);
	void CleanupBackBuffer();
	float GetScreenDpi() const;

	int m_lastWidth = 0;
	int m_lastHeight = 0;
	float m_scaleFactor = 1.0f;

	HDC m_hMemDC = nullptr;
	HBITMAP m_hMemBitmap = nullptr;
	GameField m_gameField;
	HFONT m_hFont;
	HBRUSH m_gridBrush;
	HBRUSH m_bgBrush;
	HBRUSH m_uiElemBgBrush;
	HPEN m_borderPen;
	RECT m_rcGameField;
	RECT m_rcScore;
	RECT m_rcHighScore;
	RECT m_rcNextTetramino;

	std::unique_ptr<Button> m_pauseButton;
	std::unique_ptr<Button> m_quitButton;

	int m_blockSize;
	int m_gridOffsetX;
	int m_gridOffsetY;
	int m_uiElemWidth;
	int m_uiElemHeight;
	int m_uiElemCornerRad;
	int m_uiElemSpacing;
	int m_nextWndHeight;

	static constexpr COLORREF s_textColor = RGB(128, 128, 128);
};