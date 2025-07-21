#pragma once

#include <Windows.h>
#include <cstdint>

template <typename Derived_T>
class Base {
public:
	Base() : m_window(nullptr) {}
	virtual ~Base() = default;

	Base(const Base&) = delete;
	Base& operator=(const Base&) = delete;

	bool Create(
		const wchar_t* windowName,
		uint32_t style,
		uint32_t exStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int width = CW_USEDEFAULT,
		int height = CW_USEDEFAULT,
		HBRUSH bgBrush = nullptr,
		HWND parentWnd = nullptr,
		HMENU menu = nullptr
	);

	static intptr_t _stdcall WndProc(HWND window, uint32_t msg, uintptr_t wParam, intptr_t lParam);
	HWND Window() const noexcept;
protected:
	virtual const wchar_t* ClassName() const = 0;
	virtual intptr_t HandleMessage(uint32_t msg, uintptr_t wParam, intptr_t lParam) = 0;
protected:
	HWND m_window;
};

template <typename Derived_T>
inline bool Base<Derived_T>::Create(
	const wchar_t* windowName,
	uint32_t style,
	uint32_t exStyle,
	int x,
	int y,
	int width,
	int height,
	HBRUSH bgBrush,
	HWND parentWnd,
	HMENU menu) {

	WNDCLASS wclass = { 0 };

	wclass.style = CS_HREDRAW | CS_VREDRAW;
	wclass.lpfnWndProc = Base<Derived_T>::WndProc;
	wclass.hInstance = GetModuleHandle(nullptr);
	wclass.lpszClassName = ClassName();
	wclass.hbrBackground = bgBrush;

	if (!RegisterClass(&wclass) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
		return false;
	}

	m_window = CreateWindowEx(
		exStyle, ClassName(), windowName, style, x, y,
		width, height, parentWnd, menu, GetModuleHandle(nullptr), this
	);

	return m_window != nullptr;
}


template <typename Derived_T>
inline intptr_t Base<Derived_T>::WndProc(HWND window, uint32_t msg, uintptr_t wParam, intptr_t lParam) {
	Derived_T* self = nullptr;

	if (msg == WM_NCCREATE) {
		auto* create = reinterpret_cast<CREATESTRUCT*>(lParam);
		self = static_cast<Derived_T*>(create->lpCreateParams);
		SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<intptr_t>(self));
		self->m_window = window;
	} else {
		self = reinterpret_cast<Derived_T*>(GetWindowLongPtr(window, GWLP_USERDATA));
	}

	if (self) {
		return self->HandleMessage(msg, wParam, lParam);
	}

	return DefWindowProc(window, msg, wParam, lParam);
}

template <typename Derived_T>
inline HWND Base<Derived_T>::Window() const noexcept {
	return m_window;
}
