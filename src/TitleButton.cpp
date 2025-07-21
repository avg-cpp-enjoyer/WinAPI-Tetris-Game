#include "TitleButton.h"

TitleButton::TitleButton(
	HWND parent, const std::wstring& text, const D2D1_RECT_F& bounds, float cornerRadius,
	bool borderless, const D2D1_COLOR_F& textColor, const D2D1_COLOR_F& borderColor,
	const D2D1_COLOR_F& defaultColor, const D2D1_COLOR_F& clickedColor, const D2D1_COLOR_F& hoveredColor,
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat, ID2D1RenderTarget* renderTarget
) : Button(parent, text, bounds, cornerRadius, borderless, textColor, borderColor, defaultColor,
	clickedColor, hoveredColor, textFormat, renderTarget) {

	m_renderTarget->GetFactory(&m_factory);
	m_factory->CreatePathGeometry(&m_geometry);
	m_geometry->Open(&m_sink);

	m_sink->BeginFigure(D2D1::Point2F(m_bounds.left, m_bounds.top), D2D1_FIGURE_BEGIN_FILLED);
	m_sink->AddLine(D2D1::Point2F(m_bounds.right - m_cornerRadius, m_bounds.top));
	m_sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(m_bounds.right, m_bounds.top + m_cornerRadius), D2D1::SizeF(m_cornerRadius, m_cornerRadius), 0.0f,
		D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	m_sink->AddLine(D2D1::Point2F(m_bounds.right, m_bounds.bottom));
	m_sink->AddLine(D2D1::Point2F(m_bounds.left, m_bounds.bottom));
	m_sink->AddLine(D2D1::Point2F(m_bounds.left, m_bounds.top));

	m_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	m_sink->Close();
}

TitleButton::~TitleButton() {
	if (IsWindow(m_button)) {
		SetWindowLongPtrW(m_button, GWLP_WNDPROC, reinterpret_cast<intptr_t>(m_oldProc));
	}
}

void TitleButton::Draw() {
	ID2D1SolidColorBrush* currentBrush = GetCurrentBrush();

	m_renderTarget->FillGeometry(m_geometry.Get(), currentBrush);
	m_renderTarget->DrawTextW(m_text.c_str(), static_cast<uint32_t>(m_text.length()), m_textFormat.Get(), m_bounds, m_textBrush.Get());
}
