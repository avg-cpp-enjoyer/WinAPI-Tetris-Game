#include "TetrisWindow.h"

int APIENTRY wWinMain([[maybe_unused]] HINSTANCE hInstance, HINSTANCE, [[maybe_unused]] PWSTR pCmdLine, int nCmdShow) {
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	TetrisWindow win;
	if (!win.Create(L"Tetris", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, win.windowWidth, win.windowHeight)) {
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	ResourceManager::ClearResources();
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}