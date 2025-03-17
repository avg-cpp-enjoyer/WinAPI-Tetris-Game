#pragma once

#include <Windows.h>
#include <string>
#include <functional>

class Button {
public:
	using ClickHandler = std::function<void()>;

	Button(
		HWND parent, const std::wstring& text, int x, int y, int width, int height,
		int cornerRaduis, int id, HFONT hFont, COLORREF textColor, COLORREF clrDefault,
		COLORREF clrClicked, COLORREF clrHovered
	);

	~Button();

	void Draw(HDC hdc);
	HWND GetHandle() const;
	void SetOnClick(ClickHandler handler);
	void SetText(const std::wstring& newText);

	static LRESULT CALLBACK ButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	std::wstring m_text;
	HFONT m_hFont;
	HWND m_hButton;
	int m_id;
	int m_cornerRadius;
	bool m_hovered = false, m_pressed = false;
	COLORREF m_clrDefault, m_clrClicked, m_clrHovered;
	COLORREF m_textColor;
	WNDPROC m_originalWndProc = nullptr;
	ClickHandler m_onClickHandler;
	HBRUSH m_defaultBrush = nullptr;
	HBRUSH m_hoveredBrush = nullptr;
	HBRUSH m_clickedBrush = nullptr;
	HPEN m_pen = nullptr;
};