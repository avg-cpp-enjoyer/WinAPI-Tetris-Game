#include "Button.h"

Button::Button(
	HWND parent, const std::wstring& text, const D2D1_RECT_F& bounds, float cornerRadius, 
	bool borderless, const D2D1_COLOR_F& textColor, const D2D1_COLOR_F& borderColor, 
	const D2D1_COLOR_F& defaultColor, const D2D1_COLOR_F& clickedColor, const D2D1_COLOR_F& hoveredColor, 
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat, ID2D1RenderTarget* renderTarget
) : m_text(text), m_bounds(bounds), m_cornerRadius(cornerRadius), m_borderless(borderless), 
	m_defaultColor(defaultColor), m_clickedColor(clickedColor), m_hoveredColor(hoveredColor), 
	m_borderColor(borderColor), m_textColor(textColor), m_textFormat(std::move(textFormat)), 
	m_renderTarget(renderTarget) { 

	m_renderTarget->CreateSolidColorBrush(m_defaultColor, &m_defaultBrush);
	m_renderTarget->CreateSolidColorBrush(m_clickedColor, &m_clickedBrush); 
	m_renderTarget->CreateSolidColorBrush(m_hoveredColor, &m_hoveredBrush); 
	m_renderTarget->CreateSolidColorBrush(m_borderColor, &m_borderBrush); 
	m_renderTarget->CreateSolidColorBrush(m_textColor, &m_textBrush); 

	m_boundsPx.left   = static_cast<long>(ceil(bounds.left)); 
	m_boundsPx.top    = static_cast<long>(ceil(bounds.top)); 
	m_boundsPx.right  = static_cast<long>(floor(bounds.right)); 
	m_boundsPx.bottom = static_cast<long>(floor(bounds.bottom)); 

	m_button = CreateWindowExW(0, L"BUTTON", nullptr, WS_CHILD | WS_VISIBLE, 
		m_boundsPx.left, m_boundsPx.top, m_boundsPx.right - m_boundsPx.left, 
		m_boundsPx.bottom - m_boundsPx.top, parent, 0, 
		GetModuleHandle(nullptr), nullptr); 

	SetWindowLongPtrW(m_button, GWLP_USERDATA, reinterpret_cast<intptr_t>(this)); 
	m_oldProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(m_button, GWLP_WNDPROC, reinterpret_cast<intptr_t>(ButtonProc))); 
} 

Button::~Button() { 
	if (IsWindow(m_button)) { 
		SetWindowLongPtrW(m_button, GWLP_WNDPROC, reinterpret_cast<intptr_t>(m_oldProc)); 
	} 
} 

HWND Button::GetHandle() const noexcept { 
	return m_button; 
} 

void Button::SetText(const std::wstring& newText) { 
	m_text = newText; 
	InvalidateRect(GetParent(m_button), &m_boundsPx, false); 
} 

void Button::SetOnClick(std::function<void()> handler) { 
	m_onClick = std::move(handler); 
} 

void Button::Draw() {
	static const float strokeWidth = Constants::strokeWidth;
	static const float offset = strokeWidth / 2.0f;

	D2D1_RECT_F rect = D2D1::RectF(m_bounds.left, m_bounds.top, m_bounds.right, m_bounds.bottom);
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect, m_cornerRadius, m_cornerRadius);

	ID2D1SolidColorBrush* fillBrush = GetCurrentBrush();
	m_renderTarget->FillRoundedRectangle(roundedRect, fillBrush);

	if (!m_borderless) {
		m_renderTarget->SetTransform(D2D1::Matrix3x2F::Translation(offset, offset));
		m_renderTarget->DrawRoundedRectangle(&roundedRect, m_borderBrush.Get(), strokeWidth);
		m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	m_renderTarget->DrawTextW(m_text.c_str(), static_cast<uint32_t>(m_text.length()), m_textFormat.Get(), m_bounds, m_textBrush.Get());
}

intptr_t __stdcall Button::ButtonProc(HWND window, uint32_t msg, uintptr_t wParam, intptr_t lParam) { 
	auto* self = reinterpret_cast<Button*>(GetWindowLongPtrW(window, GWLP_USERDATA)); 
	if (!self) return DefWindowProcW(window, msg, wParam, lParam); 
	
	switch (msg) { 
	case WM_MOUSEMOVE: 
		self->OnMouseMove(window); 
		return 0; 
	case WM_MOUSELEAVE: 
		self->OnMouseLeave(window); 
		return 0; 
	case WM_LBUTTONDOWN: 
		self->OnLButtonDown(window); 
		return 0; 
	case WM_LBUTTONUP: 
		self->OnLButtonUp(window); 
		return 0; 
	case WM_ERASEBKGND: 
		return 1; 
	default: 
		return CallWindowProcW(self->m_oldProc, window, msg, wParam, lParam); 
	} 
} 

ID2D1SolidColorBrush* Button::GetCurrentBrush() const {
	if (m_clicked) {
		return m_clickedBrush.Get();
	}

	if (m_hovered) {
		return m_hoveredBrush.Get();
	}

	return m_defaultBrush.Get();
}

void Button::OnMouseMove(HWND window) {
	if (!m_hovered) { 
		TRACKMOUSEEVENT tme = { sizeof(tme) };
		tme.dwFlags = TME_LEAVE; 
		tme.hwndTrack = window; 
		TrackMouseEvent(&tme); 
		m_hovered = true; 
		InvalidateRect(GetParent(window), &m_boundsPx, false); 
	} 
} 

void Button::OnMouseLeave(HWND window) { 
	m_hovered = false; 
	InvalidateRect(GetParent(window), &m_boundsPx, false); 
} 

void Button::OnLButtonDown(HWND window) { 
	m_clicked = true; 
	InvalidateRect(GetParent(window), &m_boundsPx, false); 
} 

void Button::OnLButtonUp(HWND window) { 
	m_clicked = false; 
	if (m_onClick) m_onClick(); 
	InvalidateRect(GetParent(window), &m_boundsPx, false); 
}