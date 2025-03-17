#include "TetrisWindow.h"

TetrisWindow::TetrisWindow() {
	m_hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, false, false, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Bahnschrift");

	m_gridBrush = CreateSolidBrush(RGB(50, 50, 50));
	m_bgBrush = CreateSolidBrush(RGB(30, 30, 30));
	m_uiElemBgBrush = CreateSolidBrush(RGB(40, 40, 40));
	m_borderPen = CreatePen(PS_SOLID, 0, RGB(80, 80, 80));
}

TetrisWindow::~TetrisWindow() {
	DeleteObject(m_hFont);
	DeleteObject(m_gridBrush);
	DeleteObject(m_bgBrush);
	DeleteObject(m_uiElemBgBrush);
	DeleteObject(m_borderPen);
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
		DeleteObject(m_hFont);
		DeleteObject(m_gridBrush);
		CleanupBackBuffer();
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

	RECT rcBorder = {
		GRID_OFFSET_X,
		GRID_OFFSET_Y,
		GRID_OFFSET_X + GameField::WIDTH * BLOCK_SIZE,
		GRID_OFFSET_Y + GameField::HEIGHT * BLOCK_SIZE
	};

	Rectangle(
		m_hMemDC,
		rcBorder.left,
		rcBorder.top,
		rcBorder.right,
		rcBorder.bottom
	);

	SelectObject(m_hMemDC, hOldPen);
	SelectObject(m_hMemDC, hOldBrush);

	Gdiplus::Graphics graphics(m_hMemDC);

	for (int y = 0; y < GameField::HEIGHT; y++) {
		for (int x = 0; x < GameField::WIDTH; x++) {
			TetraminoType type = m_gameField.GetGrid()[x][y];
			if (type != TetraminoType::TETRAMINO_NONE) {
				if (auto* bitmap = ResourceManager::GetTetraminoBitmap(type)) {
					graphics.DrawImage(bitmap,
						GRID_OFFSET_X + x * BLOCK_SIZE,
						GRID_OFFSET_Y + y * BLOCK_SIZE,
						BLOCK_SIZE, BLOCK_SIZE);
				}
			}
		}
	}

	if (const auto piece = m_gameField.GetCurrentTetramino()) {
		TetraminoType pieceType = piece->GetType();
		if (auto* bitmap = ResourceManager::GetTetraminoBitmap(pieceType)) {
			for (const auto& block : piece->GetTetramino()) {
				vec2 pos = piece->GetPos() + block;
				graphics.DrawImage(bitmap,
					GRID_OFFSET_X + pos.x * BLOCK_SIZE,
					GRID_OFFSET_Y + pos.y * BLOCK_SIZE,
					BLOCK_SIZE, BLOCK_SIZE);
			}
		}
	}

	const int x = GRID_OFFSET_X * 2 + GameField::WIDTH * BLOCK_SIZE;
	const int scoreY = GRID_OFFSET_Y + NEXT_WND_HEIGHT + UI_ELEM_SPACING;
	const int highScoreY = scoreY + UI_ELEM_HEIGHT + UI_ELEM_SPACING;

	DrawNextTetraminoWnd(m_hMemDC, x, GRID_OFFSET_Y);

	std::wstring scoreText = L"Score: " + std::to_wstring(m_gameField.GetScore());
	std::wstring highScoreText = L"High: " + std::to_wstring(m_gameField.GetHighScore());

	DrawRoundedText(m_hMemDC, x, scoreY, scoreText);
	DrawRoundedText(m_hMemDC, x, highScoreY, highScoreText);

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

	InvalidateRect(m_hWnd, nullptr, false);
}

void TetrisWindow::UpdateGame() {
	if (!m_gameField.IsGameOver()) {
		m_gameField.Update();
		InvalidateRect(m_hWnd, nullptr, false);
	}
}

void TetrisWindow::DrawRoundedText(HDC hdc, int x, int y, const std::wstring& text) {
	SelectObject(hdc, m_hFont);

	RECT rect = {x, y, x + UI_ELEM_WIDTH, y + UI_ELEM_HEIGHT};

	HGDIOBJ oldBrush = SelectObject(hdc, m_uiElemBgBrush);
	HGDIOBJ oldPen = SelectObject(hdc, m_borderPen);

	RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, UI_ELEM_CORNER_RAD, UI_ELEM_CORNER_RAD);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, TEXT_COLOR);

	SIZE textSize;
	GetTextExtentPoint32W(hdc, text.c_str(), (int)text.length(), &textSize);

	int posx = rect.left + (rect.right - rect.left - textSize.cx) / 2;
	int posy = rect.top + (rect.bottom - rect.top - textSize.cy) / 2;

	TextOut(hdc, posx, posy, text.c_str(), (int)text.length());

	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);
}

void TetrisWindow::DrawNextTetraminoWnd(HDC hdc, int x, int y) {

	RECT nextTetraminoRectOuter = { x, y, x + UI_ELEM_WIDTH, y + NEXT_WND_HEIGHT };

	HGDIOBJ hOldBrush = SelectObject(hdc, m_uiElemBgBrush);
	HGDIOBJ hOldPen = SelectObject(hdc, m_borderPen);

	RoundRect(
		hdc,
		nextTetraminoRectOuter.left,
		nextTetraminoRectOuter.top,
		nextTetraminoRectOuter.right,
		nextTetraminoRectOuter.bottom,
		UI_ELEM_CORNER_RAD,
		UI_ELEM_CORNER_RAD
	);

	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);

	const int padding = 10;
	RECT nextTetraminoRectInner = {
		nextTetraminoRectOuter.left + padding,
		nextTetraminoRectOuter.top + 40 + padding,
		nextTetraminoRectOuter.right - padding,
		nextTetraminoRectOuter.bottom - padding
	};

	hOldBrush = SelectObject(hdc, m_bgBrush);
	hOldPen = SelectObject(hdc, m_borderPen);

	RoundRect(
		hdc,
		nextTetraminoRectInner.left,
		nextTetraminoRectInner.top,
		nextTetraminoRectInner.right,
		nextTetraminoRectInner.bottom,
		UI_ELEM_CORNER_RAD,
		UI_ELEM_CORNER_RAD
	);

	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);

	SIZE textSize;
	GetTextExtentPoint32W(hdc, L"Next: ", 5, &textSize);
	int posy = y - 2 + (nextTetraminoRectInner.top - y - textSize.cy) / 2;

	SelectObject(hdc, m_hFont);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, TEXT_COLOR);
	TextOut(hdc, x + padding, posy, L"Next:", 5);


	Gdiplus::Graphics graphics(hdc);

	if (const auto tetramino = m_gameField.GetNextTetramino()) {
		TetraminoType tetraminoType = tetramino->GetType();
		if (auto* bitmap = ResourceManager::GetTetraminoBitmap(tetraminoType)) {
			for (const auto& block : tetramino->GetTetramino()) {
				vec2 pos = tetramino->GetPos() + block;
				int offsetX = 0;
				int offsetY = 0;

				switch (tetraminoType) {
				case TetraminoType::TETRAMINO_I:
					offsetX = -77;
					offsetY = 40;
					break;
				case TetraminoType::TETRAMINO_J:
					offsetX = -47;
					offsetY = 9;
					break;
				case TetraminoType::TETRAMINO_L:
					offsetX = -76;
					offsetY = 9;
					break;
				case TetraminoType::TETRAMINO_O:
					offsetX = -48;
					offsetY = 24;
					break;
				case TetraminoType::TETRAMINO_S:
					offsetX = -62;
					offsetY = 25;
					break;
				case TetraminoType::TETRAMINO_Z:
					offsetX = -62;
					offsetY = 25;
					break;
				case TetraminoType::TETRAMINO_T:
					offsetX = -62;
					offsetY = 26;
					break;
				}

				graphics.DrawImage(bitmap,
					nextTetraminoRectInner.left + offsetX + pos.x * BLOCK_SIZE,
					nextTetraminoRectInner.top + offsetY + pos.y * BLOCK_SIZE,
					BLOCK_SIZE, BLOCK_SIZE);
			}
		}
	}
}

void TetrisWindow::AddButtons() {
	COLORREF clrDefault = RGB(40, 40, 40);
	COLORREF clrHovered = RGB(30, 30, 30);
	COLORREF clrClicked = RGB(40, 40, 40);

	const int buttonX = GRID_OFFSET_X * 2 + GameField::WIDTH * BLOCK_SIZE;
	const int pauseBtnY = GRID_OFFSET_Y + GameField::HEIGHT * BLOCK_SIZE - UI_ELEM_HEIGHT * 2 - UI_ELEM_SPACING;
	const int quitBtnY = GRID_OFFSET_Y + GameField::HEIGHT * BLOCK_SIZE - UI_ELEM_HEIGHT;

	m_pauseButton = std::make_unique<Button>(m_hWnd, L"Pause", buttonX, pauseBtnY, UI_ELEM_WIDTH, UI_ELEM_HEIGHT, UI_ELEM_CORNER_RAD, 
		IDC_BUTTON_1, m_hFont, TEXT_COLOR, clrDefault, clrClicked, clrHovered);
	m_quitButton = std::make_unique<Button>(m_hWnd, L"Quit", buttonX, quitBtnY, UI_ELEM_WIDTH, UI_ELEM_HEIGHT, UI_ELEM_CORNER_RAD,
		IDC_BUTTON_2, m_hFont, TEXT_COLOR, clrDefault, clrClicked, clrHovered);

	m_quitButton->SetOnClick([]() { PostQuitMessage(0); });
	m_pauseButton->SetOnClick([this]() { PauseGame(); });
}

void TetrisWindow::CreateGameOverWindow() {
	KillTimer(m_hWnd, 1);

	GameOverWindow gameOver(m_gameField.GetScore(), m_gameField.GetHighScore());
	gameOver.Create(L"Game Over", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN,
		0, 0, 0, 350, 250);

	EnableWindow(m_hWnd, false);

	RECT rcMain, rcGameOver;
	GetWindowRect(m_hWnd, &rcMain);
	GetWindowRect(gameOver.Window(), &rcGameOver);

	int x = rcMain.left + (rcMain.right - rcMain.left - (rcGameOver.right - rcGameOver.left)) / 2;
	int y = rcMain.top + (rcMain.bottom - rcMain.top - (rcGameOver.bottom - rcGameOver.top)) / 2;

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

	InvalidateRect(m_hWnd, nullptr, false);
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