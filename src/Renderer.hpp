#pragma once

#include "Constants.h"
#include "GameField.h"
#include "ResourceManager.h"

#include <d2d1.h>
#include <d2d1_2.h>
#include <wrl/client.h>

class Renderer {
public:
	Renderer(
		ID2D1DeviceContext1* context,
		ID2D1Factory1* factory,
		ID2D1SolidColorBrush* bgBrush,
		ID2D1SolidColorBrush* borderBrush,
		ID2D1SolidColorBrush* uiBrush,
		ID2D1SolidColorBrush* textBrush,
		IDWriteTextFormat* textFormat,
		const RECT& windowRect
	);

	~Renderer();

	void RenderMainWindow(const RECT& rect, const GameField& gameField) const;
	void UpdateAnimations(float deltaTime, GameField& gameField);
private:
	void RenderWindow(const RECT& rect) const;
	void RenderTitleBar(const RECT& rect) const;
	void RenderGameField(const GameField& gameField) const;
	void RenderCurrentTetramino(const GameField& gameField) const;
	void RenderGhostTetramino(const GameField& gameField) const;
	void RenderNextTetramino(const GameField& gameField) const;
	void DrawRoundedText(const D2D1_ROUNDED_RECT& rect, const std::wstring& text) const;
	void RenderScore(const GameField& gameField) const;
	void RenderHighScore(const GameField& gameField) const;
	void CreateTitleBarFigure(const RECT& rect);
private:
	ID2D1DeviceContext1* m_context;
	ID2D1Factory1* m_factory;
	ID2D1SolidColorBrush* m_bgBrush;
	ID2D1SolidColorBrush* m_borderBrush;
	ID2D1SolidColorBrush* m_uiBrush;
	ID2D1SolidColorBrush* m_textBrush;
	IDWriteTextFormat* m_textFormat;

	Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_path;
};