#pragma once

#include <Windows.h>

template <typename Derived_T>
class Base {
public:
	Base() : m_hWnd(nullptr) {}
	virtual ~Base() {}

	Base(const Base&) = delete;
	Base& operator=(const Base&) = delete;

	bool Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int width = CW_USEDEFAULT,
		int height = CW_USEDEFAULT,
		HBRUSH bgColor = HBRUSH(),
		HWND hWndParent = nullptr,
		HMENU hMenu = nullptr
	);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND Window() const noexcept;

protected:
	virtual PCWSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hWnd;
};

template <typename Derived_T>
inline bool Base<Derived_T>::Create(
	PCWSTR lpWindowName,
	DWORD dwStyle,
	DWORD dwExStyle,
	int x,
	int y,
	int width,
	int height,
	HBRUSH bgColor,
	HWND hWndParent,
	HMENU hMenu) {
	WNDCLASS wclass = { 0 };

	wclass.lpfnWndProc = Base<Derived_T>::WndProc;
	wclass.hInstance = GetModuleHandle(nullptr);
	wclass.lpszClassName = ClassName();
	wclass.hbrBackground = bgColor;

	if (!RegisterClass(&wclass) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
		return false;
	}

	m_hWnd = CreateWindowEx(
		dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
		width, height, hWndParent, hMenu, GetModuleHandle(nullptr), this
	);

	return m_hWnd != nullptr;
}


template <typename Derived_T>
inline LRESULT Base<Derived_T>::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Derived_T* window = nullptr;

	if (uMsg == WM_NCCREATE) {
		CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
		window = static_cast<Derived_T*>(create->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		window->m_hWnd = hWnd;
	} else {
		window = reinterpret_cast<Derived_T*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (window) {
		return window->HandleMessage(uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

template <typename Derived_T>
inline HWND Base<Derived_T>::Window() const noexcept {
	return m_hWnd;
}