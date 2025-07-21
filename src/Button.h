#pragma once

#include "Constants.h"

#include <Windows.h>
#include <d2d1_2.h>
#include <dwrite.h>
#include <wrl/client.h>
#include <string>
#include <functional>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

class Button {
public:
	Button(
		HWND parent, const std::wstring& text, const D2D1_RECT_F& bounds, float cornerRadius,
		bool borderless, const D2D1_COLOR_F& textColor, const D2D1_COLOR_F& borderColor,
		const D2D1_COLOR_F& defaultColor, const D2D1_COLOR_F& clickedColor, const D2D1_COLOR_F& hoveredColor,
		Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat, ID2D1RenderTarget* renderTarget
	);

	virtual ~Button();

	HWND GetHandle() const noexcept;
	void SetText(const std::wstring& newText);
	void SetOnClick(std::function<void()> handler);
	virtual void Draw();
private:
	void OnMouseMove(HWND window);
	void OnMouseLeave(HWND window);
	void OnLButtonDown(HWND window);
	void OnLButtonUp(HWND window);
	static intptr_t __stdcall ButtonProc(HWND window, uint32_t msg, uintptr_t wParam, intptr_t lParam);
protected:
	ID2D1SolidColorBrush* GetCurrentBrush() const;
protected:
	std::wstring m_text;
	D2D1_RECT_F m_bounds;
	RECT m_boundsPx;
	float m_cornerRadius;
	bool m_borderless;
	bool m_hovered = false;
	bool m_clicked = false;

	D2D1_COLOR_F m_defaultColor;
	D2D1_COLOR_F m_clickedColor;
	D2D1_COLOR_F m_hoveredColor;
	D2D1_COLOR_F m_borderColor;
	D2D1_COLOR_F m_textColor;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_defaultBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_hoveredBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_clickedBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_borderBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_textBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat>    m_textFormat;

	ID2D1RenderTarget* m_renderTarget;

	HWND m_button;
	WNDPROC m_oldProc;
	std::function<void()> m_onClick;
};