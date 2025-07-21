#include "TetrisWindow.h"

int _stdcall wWinMain([[maybe_unused]] _In_ HINSTANCE instance, _In_opt_ HINSTANCE, [[maybe_unused]] _In_ wchar_t* cmdLine, _In_ int cmdShow) {
	long hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	if (FAILED(hr)) {
		MessageBox(nullptr, L"CoInitializeEx failed", L"Error", MB_ICONERROR);
		return -1;
	}

	Constants::Init();

	TetrisWindow tetris;
	if (!tetris.Create(L"Tetris", WS_POPUP | WS_VISIBLE, WS_EX_APPWINDOW | WS_EX_NOREDIRECTIONBITMAP, CW_USEDEFAULT, CW_USEDEFAULT,
		Constants::windowWidth, Constants::windowHeight)) {
		return 0;
	}

	tetris.Exec(cmdShow);

	CoUninitialize();
	return 0;
}