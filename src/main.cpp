#include "TetrisWindow.h"

#pragma comment(lib, "Winmm.lib")

int _stdcall wWinMain([[maybe_unused]] _In_ HINSTANCE instance, _In_opt_ HINSTANCE, [[maybe_unused]] _In_ wchar_t* cmdLine, _In_ int cmdShow) {
	timeBeginPeriod(1);
	long hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	if (FAILED(hr)) {
		MessageBox(nullptr, L"CoInitializeEx failed", L"Error", MB_ICONERROR);
		return -1;
	}

	UI::Init();

	TetrisWindow tetris;
	if (!tetris.Create(L"Tetris", WS_POPUP | WS_VISIBLE, WS_EX_APPWINDOW | WS_EX_NOREDIRECTIONBITMAP, CW_USEDEFAULT, CW_USEDEFAULT,
		UI::MainWindow::mwWidth, UI::MainWindow::mwHeight)) {
		return 0;
	}

	tetris.Exec(cmdShow);

	CoUninitialize();
	timeEndPeriod(1);
	return 0;
}