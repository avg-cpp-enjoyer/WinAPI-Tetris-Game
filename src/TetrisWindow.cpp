#include "TetrisWindow.h"

TetrisWindow::TetrisWindow() {
	m_gridBrush = CreateSolidBrush(RGB(50, 50, 50));
	m_bgBrush = CreateSolidBrush(RGB(30, 30, 30));
	m_uiElemBgBrush = CreateSolidBrush(RGB(40, 40, 40));
	m_borderPen = CreatePen(PS_SOLID, 0, RGB(80, 80, 80));

	m_scaleFactor = GetScreenDpi() / 96.0f;

	m_hFont = CreateFont(static_cast<int>(24 * m_scaleFactor), 0, 0, 0, FW_NORMAL, false, false, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Bahnschrift");

	m_blockSize       = static_cast<int>(30.0f  * m_scaleFactor);
	m_gridOffsetX     = static_cast<int>(20.0f  * m_scaleFactor);
	m_gridOffsetY     = static_cast<int>(20.0f  * m_scaleFactor);
	m_uiElemWidth     = static_cast<int>(168.0f * m_scaleFactor);
	m_uiElemHeight    = static_cast<int>(44.0f  * m_scaleFactor);
	m_uiElemCornerRad = static_cast<int>(10.0f  * m_scaleFactor);
	m_uiElemSpacing   = static_cast<int>(15.0f  * m_scaleFactor);
	m_nextWndHeight   = static_cast<int>(168.0f * m_scaleFactor);
	windowWidth       = static_cast<int>(544.0f * m_scaleFactor);
	windowHeight      = static_cast<int>(678.0f * m_scaleFactor);

	m_rcGameField = {
		m_gridOffsetX, m_gridOffsetY,
		m_gridOffsetX + GameField::WIDTH * m_blockSize,
		m_gridOffsetY + GameField::HEIGHT * m_blockSize
	};

	m_rcScore = {
		m_gridOffsetX * 2 + GameField::WIDTH * m_blockSize,
		m_gridOffsetY + m_nextWndHeight + m_uiElemSpacing,
		m_gridOffsetX * 2 + GameField::WIDTH * m_blockSize + m_uiElemWidth,
		m_gridOffsetY + m_nextWndHeight + m_uiElemSpacing + m_uiElemHeight
	};

	m_rcHighScore = {
		m_gridOffsetX * 2 + GameField::WIDTH * m_blockSize,
		m_gridOffsetY + m_nextWndHeight + m_uiElemSpacing * 2 + m_uiElemHeight,
		m_gridOffsetX * 2 + GameField::WIDTH * m_blockSize + m_uiElemWidth,
		m_gridOffsetY + m_nextWndHeight + m_uiElemSpacing * 2 + m_uiElemHeight * 2
	};

	m_rcNextTetramino = {
		m_gridOffsetX * 2 + GameField::WIDTH * m_blockSize, m_gridOffsetY,
		m_gridOffsetX * 2 + GameField::WIDTH * m_blockSize + m_uiElemWidth,
		m_gridOffsetY + m_nextWndHeight
	};
}

TetrisWindow::~TetrisWindow() {
	DeleteObject(m_hFont);
	DeleteObject(m_gridBrush);
	DeleteObject(m_bgBrush);
	DeleteObject(m_uiElemBgBrush);
	DeleteObject(m_borderPen);
	CleanupBackBuffer();
}

PCWSTR TetrisWindow::ClassName() const {
	return L"TetrisWindowClass";
}

LRESULT TetrisWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:
		SetTimer(m_hWnd, 1, 500, nullptr);
		AddButtons();
		return 0;
	case WM_DESTROY:
		KillTimer(m_hWnd, 1);
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		DestroyWindow(m_hWnd);
		break;
	case WM_PAINT:
		OnPaint();
		return 0;
	case WM_KEYDOWN:
		OnKeyDown(wParam);
		return 0;
	case WM_TIMER:
		if (!m_gameField.IsGameOver()) {
			UpdateGame();
		} else {
			CreateGameOverWindow();
		}
		break;
	}

	return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

void TetrisWindow::OnPaint() {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hWnd, &ps);

	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);
	CreateBackBuffer(rcClient.right, rcClient.bottom);
	FillRect(m_hMemDC, &rcClient, m_bgBrush);

	HGDIOBJ hOldPen = SelectObject(m_hMemDC, m_borderPen);
	HGDIOBJ hOldBrush = SelectObject(m_hMemDC, GetStockObject(HOLLOW_BRUSH));

	Rectangle(
		m_hMemDC,
		m_rcGameField.left,
		m_rcGameField.top,
		m_rcGameField.right,
		m_rcGameField.bottom
	);

	SelectObject(m_hMemDC, hOldPen);
	SelectObject(m_hMemDC, hOldBrush);

	Gdiplus::Graphics graphics(m_hMemDC);

	for (int y = 0; y < GameField::HEIGHT; y++) {
		for (int x = 0; x < GameField::WIDTH; x++) {
			TetraminoType tetraminoType = m_gameField.GetGrid()[x][y];
			if (tetraminoType != TetraminoType::TETRAMINO_NONE) {
				const auto bitmap = ResourceManager::GetTetraminoBitmap(tetraminoType);
				graphics.DrawImage(bitmap,
					m_gridOffsetX + x * m_blockSize,
					m_gridOffsetY + y * m_blockSize,
					m_blockSize, m_blockSize);
			}
		}
	}

	const auto tetramino = m_gameField.GetCurrentTetramino();
	const auto bitmap = ResourceManager::GetTetraminoBitmap(tetramino->GetType());
	for (const auto& block : tetramino->GetTetramino()) {
		vec2 pos = tetramino->GetPos() + block;
		graphics.DrawImage(bitmap,
			m_gridOffsetX + pos.x * m_blockSize,
			m_gridOffsetY + pos.y * m_blockSize,
			m_blockSize, m_blockSize);
	}
		

	if (!m_gameField.IsGhostCollide()) {
		const auto ghost = m_gameField.GetGhostTetramino();
		const auto ghostBitmap = ResourceManager::GetTetraminoBitmap(TetraminoType::TETRAMINO_GHOST);
		for (const auto& block : ghost->GetTetramino()) {
			vec2 pos = ghost->GetPos() + block;
			graphics.DrawImage(ghostBitmap,
				m_gridOffsetX + pos.x * m_blockSize,
				m_gridOffsetY + pos.y * m_blockSize,
				m_blockSize, m_blockSize);
		}
	}

	DrawNextTetraminoWnd(m_hMemDC, m_rcNextTetramino);

	std::wstring scoreText = L"Score: " + std::to_wstring(m_gameField.GetScore());
	std::wstring highScoreText = L"High: " + std::to_wstring(m_gameField.GetHighScore());

	DrawRoundedText(m_hMemDC, m_rcScore, scoreText);
	DrawRoundedText(m_hMemDC, m_rcHighScore, highScoreText);

	BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, m_hMemDC, 0, 0, SRCCOPY);
	EndPaint(m_hWnd, &ps);
}

void TetrisWindow::OnKeyDown(WPARAM key) {
	if (m_gameField.IsGameOver() || m_gameField.IsPaused()) {
		return;
	}

	switch (key) {
	case VK_LEFT:  
		m_gameField.MoveCurrent(Direction::DIRECTION_LEFT); 
		break;
	case VK_RIGHT: 
		m_gameField.MoveCurrent(Direction::DIRECTION_RIGHT); 
		break;
	case VK_DOWN:  
		m_gameField.MoveCurrent(Direction::DIRECTION_DOWN); 
		break;
	case VK_UP:    
		m_gameField.RotateCurrent(); 
		break;
	case VK_SPACE: 
		m_gameField.HardDrop(); 
		break;
	}

	m_gameField.UpdateGhostPos();
	UpdateWnd();
}

void TetrisWindow::UpdateGame() {
	if (!m_gameField.IsGameOver()) {
		m_gameField.Update();
		UpdateWnd();
	}
}

void TetrisWindow::UpdateWnd() const {
	InvalidateRect(m_hWnd, &m_rcGameField, false);
	InvalidateRect(m_hWnd, &m_rcNextTetramino, false);
	InvalidateRect(m_hWnd, &m_rcScore, false);
	InvalidateRect(m_hWnd, &m_rcHighScore, false);
}

void TetrisWindow::DrawRoundedText(HDC hdc, RECT rect, const std::wstring& text) {
	SelectObject(hdc, m_hFont);

	HGDIOBJ oldBrush = SelectObject(hdc, m_uiElemBgBrush);
	HGDIOBJ oldPen = SelectObject(hdc, m_borderPen);

	RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, m_uiElemCornerRad, m_uiElemCornerRad);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, s_textColor);

	DrawText(hdc, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);
}

void TetrisWindow::DrawNextTetraminoWnd(HDC hdc, RECT rect) {
	HGDIOBJ hOldBrush = SelectObject(hdc, m_uiElemBgBrush);
	HGDIOBJ hOldPen = SelectObject(hdc, m_borderPen);

	RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, m_uiElemCornerRad, m_uiElemCornerRad);

	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);

	const int padding = static_cast<int>(10.0f * m_scaleFactor);
	RECT nextTetraminoRectInner = {
		rect.left + padding, rect.top + static_cast<int>(40.0f * m_scaleFactor) + padding,
		rect.right - padding, rect.bottom - padding
	};

	hOldBrush = SelectObject(hdc, m_bgBrush);
	hOldPen = SelectObject(hdc, m_borderPen);

	RoundRect(
		hdc,
		nextTetraminoRectInner.left,
		nextTetraminoRectInner.top,
		nextTetraminoRectInner.right,
		nextTetraminoRectInner.bottom,
		m_uiElemCornerRad,
		m_uiElemCornerRad
	);

	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);

	RECT textRect = {rect.left, rect.top, rect.right, nextTetraminoRectInner.top};

	SelectObject(hdc, m_hFont);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, s_textColor);
	DrawText(hdc, L"Next", -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	Gdiplus::Graphics graphics(hdc);

	const auto tetramino = m_gameField.GetNextTetramino();
	TetraminoType tetraminoType = tetramino->GetType();
	const auto bitmap = ResourceManager::GetTetraminoBitmap(tetraminoType);
	for (const auto& block : tetramino->GetTetramino()) {
		vec2 pos = tetramino->GetPos() + block;
		int offsetX = 0;
		int offsetY = 0;

		switch (tetraminoType) {
		case TetraminoType::TETRAMINO_I:
			offsetX = static_cast<int>(-77.0f * m_scaleFactor);
			offsetY = static_cast<int>(40.0f * m_scaleFactor);
			break;
		case TetraminoType::TETRAMINO_J:
			offsetX = static_cast<int>(-47.0f * m_scaleFactor);
			offsetY = static_cast<int>(9.0f * m_scaleFactor);
			break;
		case TetraminoType::TETRAMINO_L:
			offsetX = static_cast<int>(-77.0f * m_scaleFactor);
			offsetY = static_cast<int>(9.0f * m_scaleFactor);
			break;
		case TetraminoType::TETRAMINO_O:
			offsetX = static_cast<int>(-46.0f * m_scaleFactor);
			offsetY = static_cast<int>(25.0f * m_scaleFactor);
			break;
		case TetraminoType::TETRAMINO_S:
		case TetraminoType::TETRAMINO_Z: 
		case TetraminoType::TETRAMINO_T:
			offsetX = static_cast<int>(-62.0f * m_scaleFactor);
			offsetY = static_cast<int>(25.0f * m_scaleFactor);
			break;
		}

		graphics.DrawImage(bitmap,
			nextTetraminoRectInner.left + offsetX + pos.x * m_blockSize,
			nextTetraminoRectInner.top + offsetY + pos.y * m_blockSize,
			m_blockSize, m_blockSize);
	}
}

void TetrisWindow::AddButtons() {
	COLORREF clrDefault = RGB(40, 40, 40);
	COLORREF clrHovered = RGB(30, 30, 30);
	COLORREF clrClicked = RGB(40, 40, 40);

	const int buttonX = m_gridOffsetX * 2 + GameField::WIDTH * m_blockSize;
	const int pauseBtnY = m_gridOffsetY + GameField::HEIGHT * m_blockSize - m_uiElemHeight * 2 - m_uiElemSpacing;
	const int quitBtnY = m_gridOffsetY + GameField::HEIGHT * m_blockSize - m_uiElemHeight;

	m_pauseButton = std::make_unique<Button>(m_hWnd, L"Pause", buttonX, pauseBtnY, m_uiElemWidth, m_uiElemHeight, m_uiElemCornerRad, 
		IDC_BUTTON_1, m_hFont, s_textColor, clrDefault, clrClicked, clrHovered);
	m_quitButton = std::make_unique<Button>(m_hWnd, L"Quit", buttonX, quitBtnY, m_uiElemWidth, m_uiElemHeight, m_uiElemCornerRad,
		IDC_BUTTON_2, m_hFont, s_textColor, clrDefault, clrClicked, clrHovered);

	m_quitButton->SetOnClick([]() { PostQuitMessage(0); });
	m_pauseButton->SetOnClick([this]() { PauseGame(); });
}

void TetrisWindow::CreateGameOverWindow() {
	KillTimer(m_hWnd, 1);

	GameOverWindow gameOver(m_gameField.GetScore(), m_gameField.GetHighScore());
	gameOver.Create(L"Game Over", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		0, 0, 0, static_cast<int>(350.0f * m_scaleFactor), static_cast<int>(250.0f * m_scaleFactor));

	EnableWindow(m_hWnd, false);

	RECT rcMain, rcGameOver;
	GetWindowRect(m_hWnd, &rcMain);
	GetWindowRect(gameOver.Window(), &rcGameOver);

	const int x = rcMain.left + (rcMain.right - rcMain.left - (rcGameOver.right - rcGameOver.left)) / 2;
	const int y = rcMain.top + (rcMain.bottom - rcMain.top - (rcGameOver.bottom - rcGameOver.top)) / 2;

	SetWindowPos(gameOver.Window(), nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	ShowWindow(gameOver.Window(), SW_SHOW);
	UpdateWindow(gameOver.Window());

	MSG msg;
	while (IsWindowVisible(gameOver.Window()) && GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	EnableWindow(m_hWnd, TRUE);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	if (gameOver.ShouldRestart()) {
		m_gameField.Reset(); 
		SetTimer(m_hWnd, 1, 500, nullptr);
		InvalidateRect(m_hWnd, nullptr, true);
	}
}

void TetrisWindow::PauseGame() {
	m_gameField.Pause();

	if (m_gameField.IsPaused()) {
		m_pauseButton->SetText(L"Continue");
		KillTimer(m_hWnd, 1);
	} else {
		m_pauseButton->SetText(L"Pause");
		SetTimer(m_hWnd, 1, 500, nullptr);
	}

	InvalidateRect(m_hWnd, &m_rcGameField, false);
	SetFocus(m_hWnd);
}

void TetrisWindow::CreateBackBuffer(int width, int height) {
	HDC hdc = GetDC(m_hWnd);

	if (m_hMemDC) {
		if (width == m_lastWidth && height == m_lastHeight) return;
		CleanupBackBuffer();
	}

	m_hMemDC = CreateCompatibleDC(hdc);
	m_hMemBitmap = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(m_hMemDC, m_hMemBitmap);

	m_lastWidth = width;
	m_lastHeight = height;

	ReleaseDC(m_hWnd, hdc);
}

void TetrisWindow::CleanupBackBuffer() {
	if (m_hMemBitmap) DeleteObject(m_hMemBitmap);
	if (m_hMemDC) DeleteDC(m_hMemDC);
	m_hMemDC = nullptr;
	m_hMemBitmap = nullptr;
}

float TetrisWindow::GetScreenDpi() const {
	UINT dpiX = 0;
	UINT dpiY = 0;

	HMONITOR hMonitor = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	HRESULT hRes = GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);

	return SUCCEEDED(hRes) ? static_cast<float>(dpiX) : 96.0f;
}
