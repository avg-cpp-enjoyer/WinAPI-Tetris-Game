#pragma once

#include "GameField.h"
#include "ResourceManager.h"
#include "GameOverWindow.h"
#include "Base.h"
#include "Button.h"

#include <unordered_map>
#include <gdiplus.h>
#include <Shlwapi.h> 

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "gdiplus.lib")

#define IDC_BUTTON_1 1001
#define IDC_BUTTON_2 1002

class TetrisWindow : public Base<TetrisWindow> {
public:
	TetrisWindow();
	~TetrisWindow();

	PCWSTR ClassName() const override;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
private:
	void OnPaint();
	void OnKeyDown(WPARAM key);
	void UpdateGame();
	void DrawRoundedText(HDC hdc, int x, int y, const std::wstring& text);
	void DrawNextTetraminoWnd(HDC hdc, int x, int y);
	void AddButtons();
	void CreateGameOverWindow();
	void PauseGame();

	HDC m_hMemDC = nullptr;
	HBITMAP m_hMemBitmap = nullptr;
	int m_lastWidth = 0;
	int m_lastHeight = 0;

	void CreateBackBuffer(int width, int height);
	void CleanupBackBuffer();

	GameField m_gameField;
	HFONT m_hFont;
	HBRUSH m_gridBrush;
	HBRUSH m_bgBrush;
	HBRUSH m_uiElemBgBrush;
	HPEN m_borderPen;

	std::unique_ptr<Button> m_pauseButton;
	std::unique_ptr<Button> m_quitButton;

	static constexpr int BLOCK_SIZE = 30;
	static constexpr int NEXT_PIECE_SIZE = 20;
	static constexpr int GRID_OFFSET_X = 20;
	static constexpr int GRID_OFFSET_Y = 20;
	static constexpr int UI_ELEM_WIDTH = 168;
	static constexpr int UI_ELEM_HEIGHT = 44;
	static constexpr int UI_ELEM_CORNER_RAD = 10;
	static constexpr int UI_ELEM_SPACING = 15;
	static constexpr int NEXT_WND_HEIGHT = 168;

	static constexpr COLORREF TEXT_COLOR = RGB(128, 128, 128);
};