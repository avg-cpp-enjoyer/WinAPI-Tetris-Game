#include "GameOverWindow.h"

#include <string>

GameOverWindow::GameOverWindow(int score, int highScore)
    : m_score(score), m_highScore(highScore) {
    m_hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, false, false, false,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Bahnschrift");

	m_bgBrush = CreateSolidBrush(RGB(30, 30, 30));
}

GameOverWindow::~GameOverWindow() {
	DeleteObject(m_hFont);
	DeleteObject(m_bgBrush);
}

PCWSTR GameOverWindow::ClassName() const { 
    return L"GameOverWindowClass"; 
}

LRESULT GameOverWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        CreateControls();
        break;
    case WM_DESTROY:
        DeleteObject(m_hFont);
        break;
	case WM_PAINT:
		OnPaint();
		break;
    default:
        return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

bool GameOverWindow::ShouldRestart() const {
	return m_restart;
}

void GameOverWindow::CreateControls() {
	RECT rc;
	GetClientRect(m_hWnd, &rc);

	const int buttonWidth = 100;
	const int buttonHeight = 40;
	const int buttonSpacing = 20;

	int totalWidth = buttonWidth * 2 + buttonSpacing;
	int startX = (rc.right - totalWidth) / 2;
	int yPos = rc.bottom - buttonHeight - 20;

	COLORREF clrDefault = RGB(50, 50, 50);
	COLORREF clrHovered = RGB(60, 60, 60);
	COLORREF clrClicked = RGB(70, 70, 70);
	COLORREF textColor  = RGB(128, 128, 128);

	m_restartButton = std::make_unique<Button>(m_hWnd, L"Restart", startX, yPos, buttonWidth, buttonHeight,
		10, IDC_BUTTON_R, m_hFont, textColor, clrDefault, clrClicked, clrHovered);

	m_quitButton = std::make_unique<Button>(m_hWnd, L"Quit", startX + buttonWidth + buttonSpacing, yPos,
		buttonWidth, buttonHeight, 10, IDC_BUTTON_Q, m_hFont, textColor, clrDefault, clrClicked, clrHovered);

	m_restartButton->SetOnClick([this]() {
		m_restart = true;
		DestroyWindow(m_hWnd);
	});

	m_quitButton->SetOnClick([this]() { 
		PostQuitMessage(0);
		DestroyWindow(GetParent(m_hWnd));  
		DestroyWindow(m_hWnd);
	});
}

void GameOverWindow::OnPaint() {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hWnd, &ps);

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	FillRect(hdc, &rc, m_bgBrush);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	SelectObject(hdc, m_hFont);

	std::wstring text = L"Game Over!\nScore: " + std::to_wstring(m_score)
		+ L"\nHigh Score: " + std::to_wstring(m_highScore);

	RECT textRect = rc;
	textRect.top = rc.top + 40;
	textRect.bottom = rc.bottom - 80;
	DrawText(hdc, text.c_str(), -1, &textRect, DT_CENTER | DT_VCENTER | DT_NOCLIP);

	EndPaint(m_hWnd, &ps);
}
