#include "Button.h"

Button::Button(
	HWND parent, const std::wstring& text, int x, int y, int width, int height,
	int cornerRadius, int id, HFONT hFont, COLORREF textColor, COLORREF clrDefault,
	COLORREF clrClicked, COLORREF clrHovered
) : m_text(text), m_hFont(hFont), m_id(id), m_cornerRadius(cornerRadius), m_textColor(textColor),
    m_clrDefault(clrDefault), m_clrClicked(clrClicked), m_clrHovered(clrHovered) {

	m_hButton = CreateWindow(
		L"BUTTON",
		text.c_str(),
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		x, y, width, height,
		parent,
		(HMENU)(INT_PTR)id,
		GetModuleHandle(nullptr),
		this
	);

	HRGN hRgn = CreateRoundRectRgn(0, 0, width, height, m_cornerRadius, m_cornerRadius);
	SetWindowRgn(m_hButton, hRgn, true); 
	DeleteObject(hRgn);

	m_defaultBrush = CreateSolidBrush(m_clrDefault);
	m_hoveredBrush = CreateSolidBrush(m_clrHovered);
	m_clickedBrush = CreateSolidBrush(m_clrClicked);
	m_pen = CreatePen(PS_SOLID, 0, RGB(80, 80, 80));

	m_originalWndProc = (WNDPROC)SetWindowLongPtr(m_hButton, GWLP_WNDPROC, (LONG_PTR)ButtonProc);
	SetWindowLongPtr(m_hButton, GWLP_USERDATA, (LONG_PTR)this);
	SendMessage(m_hButton, WM_SETFONT, (WPARAM)m_hFont, true);
}

Button::~Button() {
	SetWindowLongPtr(m_hButton, GWLP_WNDPROC, (LONG_PTR)m_originalWndProc);
	DeleteObject(m_defaultBrush);
	DeleteObject(m_hoveredBrush);
	DeleteObject(m_clickedBrush);
	DeleteObject(m_pen);
}

void Button::Draw(HDC hdc) {
	RECT rect = {};
	GetClientRect(m_hButton, &rect);

	HBRUSH currentBrush = m_defaultBrush;

	if (m_hovered) {
		currentBrush = m_hoveredBrush;
	}

	if (m_pressed) {
		currentBrush = m_clickedBrush;
	}

	HGDIOBJ oldPen = SelectObject(hdc, m_pen);
	HGDIOBJ oldBrush = SelectObject(hdc, currentBrush);

	RoundRect(
		hdc,
		rect.left, rect.top,
		rect.right, rect.bottom,
		m_cornerRadius, m_cornerRadius
	);

	HFONT oldFont = (HFONT)SelectObject(hdc, m_hFont);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, m_textColor);
	DrawText(hdc, m_text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldFont);
}

void Button::SetOnClick(ClickHandler handler) {
	m_onClickHandler = handler;
}

void Button::SetText(const std::wstring& newText) {
	m_text = newText;
	InvalidateRect(m_hButton, nullptr, true);
}

LRESULT CALLBACK Button::ButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	auto* button = reinterpret_cast<Button*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (!button) return DefWindowProc(hWnd, msg, wParam, lParam);

	switch (msg) {
	case WM_NCCREATE: {
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		button = (Button*)cs->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)button);
		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		button->Draw(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_MOUSEMOVE: {
		if (!button->m_hovered) {
			TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, hWnd, 0 };
			TrackMouseEvent(&tme);
			button->m_hovered = true;
			InvalidateRect(hWnd, nullptr, true);
		}
		break;
	}
	case WM_MOUSELEAVE: {
		button->m_hovered = false;
		InvalidateRect(hWnd, nullptr, true);
		break;
	}
	case WM_LBUTTONDOWN: {
		button->m_pressed = true;
		InvalidateRect(hWnd, nullptr, true);
		break;
	}
	case WM_LBUTTONUP: {
		button->m_pressed = false;
		button->m_onClickHandler();
		InvalidateRect(hWnd, nullptr, true);
		break;
	}
	case WM_ERASEBKGND: {
		return 1;
	}
	}

	return CallWindowProc(button->m_originalWndProc, hWnd, msg, wParam, lParam);
}

HWND Button::GetHandle() const {
	return m_hButton;
}