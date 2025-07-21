#include "GameOverWindow.h"

GameOverWindow::GameOverWindow(int score, int highScore) : m_score(score), m_highScore(highScore) {}

const wchar_t* GameOverWindow::ClassName() const {
	return L"GameOverWindowClass";
}

intptr_t GameOverWindow::HandleMessage(uint32_t msg, uintptr_t wParam, intptr_t lParam) {
	switch (msg) {
	case WM_NCCALCSIZE:
		return 0;
	case WM_NCHITTEST:
		return OnNcHitTest(lParam);
	case WM_CREATE:
		InitializeD2D();
		CreateButtons();
		RenderLayeredWindow();
		return 0;
	case WM_PAINT:
		RenderLayeredWindow();
		ValidateRect(m_window, nullptr);
		return 0;
	case WM_DESTROY:
		CleanupD2D();
		return 0;
	}

	return DefWindowProc(m_window, msg, wParam, lParam);
}

bool GameOverWindow::ShouldRestart() const noexcept {
	return m_restart;
}

intptr_t GameOverWindow::OnNcHitTest(intptr_t lParam) const {
	POINT pt = { LOWORD(lParam), HIWORD(lParam) };
	ScreenToClient(m_window, &pt);

	if (pt.y < Constants::titleBarHeight) {
		return HTCAPTION;
	}

	return DefWindowProc(m_window, WM_NCHITTEST, 0, lParam);
}

void GameOverWindow::InitializeD2D() {
	D2D1_FACTORY_OPTIONS options = {};
	HR_LOG(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &options,
		reinterpret_cast<void**>(m_d2dFactory.GetAddressOf())));

	HR_LOG(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_wicFactory)));
	HR_LOG(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_writeFactory));

	HR_LOG(m_writeFactory->CreateTextFormat(L"Bahnschrift", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, Constants::fontSize, L"", &m_textFormat));

	m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_SOFTWARE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

	HR_LOG(m_d2dFactory->CreateDCRenderTarget(&rtProps, &m_d2dRT));

	HR_LOG(m_d2dRT->CreateSolidColorBrush(Constants::bgColor, &m_bgBrush));
	HR_LOG(m_d2dRT->CreateSolidColorBrush(Constants::borderColor, &m_borderBrush));
	HR_LOG(m_d2dRT->CreateSolidColorBrush(Constants::uiTextColor, &m_textBrush));
}

void GameOverWindow::CleanupD2D() {
	m_d2dRT.Reset();
	m_d2dFactory.Reset();
	m_wicFactory.Reset();
	m_writeFactory.Reset();
	m_textFormat.Reset();
	m_bgBrush.Reset();
	m_borderBrush.Reset();
	m_textBrush.Reset();
}

void GameOverWindow::CreateButtons() {
	m_restartButton = std::make_unique<Button>(m_window, L"Restart", Constants::gameOverRestartRect, Constants::uiElemCornerRad,
		false, Constants::uiTextColor, Constants::borderColor, Constants::btnClrDefault,
		Constants::btnClrClicked, Constants::btnClrHovered, m_textFormat, m_d2dRT.Get()
	);

	m_quitButton = std::make_unique<Button>(m_window, L"Quit", Constants::gameOverQuitRect, Constants::uiElemCornerRad,
		false, Constants::uiTextColor, Constants::borderColor, Constants::btnClrDefault,
		Constants::btnClrClicked, Constants::btnClrHovered, m_textFormat, m_d2dRT.Get()
	);

	m_restartButton->SetOnClick([this]() {
		m_restart = true;
		DestroyWindow(m_window);
	});

	m_quitButton->SetOnClick([this]() {
		m_restart = false;
		DestroyWindow(m_window);
	});
}

void GameOverWindow::RenderLayeredWindow() {
	RECT rc;
	GetClientRect(m_window, &rc);

	const int width = static_cast<int>(Constants::gameOverWndWidth);
	const int height = static_cast<int>(Constants::gameOverWndHeight);

	HDC hdcScreen = GetDC(nullptr);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);

	BITMAPINFO info = {};
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = -height;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;

	void* bits = nullptr;
	HBITMAP  bitmap = CreateDIBSection(hdcScreen, &info, DIB_RGB_COLORS, &bits, nullptr, 0);

	if (!bitmap) {
		DeleteDC(hdcMem);
		ReleaseDC(nullptr, hdcScreen);
		return;
	}

	HGDIOBJ oldBmp = SelectObject(hdcMem, bitmap);

	HR_LOG(m_d2dRT->BindDC(hdcMem, &rc));

	m_d2dRT->BeginDraw();
	m_d2dRT->Clear(D2D1::ColorF(0, 0, 0, 0));

	D2D1_ROUNDED_RECT rect = {
		std::floor(static_cast<float>(rc.left))  + 1.0f,
		std::floor(static_cast<float>(rc.top))   + 1.0f,
		std::ceil(static_cast<float>(rc.right))  - 1.0f,
		std::ceil(static_cast<float>(rc.bottom)) - 1.0f,
		Constants::windowCornerRad - 0.5f,
		Constants::windowCornerRad - 0.5f
	};

	const float strokeWidth = Constants::strokeWidth;
	const float offset = strokeWidth / 2.0f;

	m_d2dRT->SetTransform(D2D1::Matrix3x2F::Translation(offset, offset));
	m_d2dRT->FillRoundedRectangle(rect, m_bgBrush.Get());
	m_d2dRT->DrawRoundedRectangle(rect, m_borderBrush.Get(), strokeWidth);
	m_d2dRT->SetTransform(D2D1::Matrix3x2F::Identity());

	std::wstring text = L"Game Over!\nScore: " + std::to_wstring(m_score) + L"\nHigh Score: " + std::to_wstring(m_highScore);

	D2D1_RECT_F textRect = D2D1::RectF(0, 0, Constants::gameOverWndWidth, Constants::gameOverWndHeight - 80);

	m_d2dRT->DrawTextW(text.c_str(), static_cast<uint32_t>(text.size()), m_textFormat.Get(), textRect, m_textBrush.Get());

	m_restartButton->Draw();
	m_quitButton->Draw();

	m_d2dRT->EndDraw();

	POINT source = { 0, 0 };
	SIZE  size = { width, height };
	BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	UpdateLayeredWindow(m_window, hdcScreen, nullptr, &size, hdcMem, &source, 0, &blend, ULW_ALPHA);

	SelectObject(hdcMem, oldBmp);
	DeleteObject(bitmap);
	DeleteDC(hdcMem);
	ReleaseDC(nullptr, hdcScreen);
}