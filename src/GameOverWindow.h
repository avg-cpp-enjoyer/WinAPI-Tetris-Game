#pragma once

#include "Base.h"
#include "Button.h"
#include "GameField.h"
#include "Constants.h"
#include "Log.hpp"

#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <memory>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "Windowscodecs.lib")

class GameOverWindow final : public Base<GameOverWindow> {
public:
	GameOverWindow(int score, int highScore);
	~GameOverWindow() override {}

	const wchar_t* ClassName() const override;
	intptr_t HandleMessage(uint32_t msg, uintptr_t wParam, intptr_t lParam) override;
	bool ShouldRestart() const noexcept;
private:
	void InitializeD2D();
	void CleanupD2D();
	void CreateButtons();
	void RenderLayeredWindow();
	intptr_t OnNcHitTest(intptr_t lParam) const;
private:
	int   m_score;
	int   m_highScore;
	bool  m_restart = false;

	Microsoft::WRL::ComPtr<ID2D1Factory1>        m_d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1DCRenderTarget>  m_d2dRT;
	Microsoft::WRL::ComPtr<IWICImagingFactory>   m_wicFactory;
	Microsoft::WRL::ComPtr<IDWriteFactory>       m_writeFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat>    m_textFormat;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_bgBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_borderBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_textBrush;

	std::unique_ptr<Button> m_restartButton;
	std::unique_ptr<Button> m_quitButton;
};
