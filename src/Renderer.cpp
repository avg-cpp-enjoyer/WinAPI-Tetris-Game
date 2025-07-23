#include "Renderer.hpp"

Renderer::Renderer(ID2D1DeviceContext1* context, ID2D1Factory1* factory, ID2D1SolidColorBrush* bgBrush,
	ID2D1SolidColorBrush* borderBrush, ID2D1SolidColorBrush* uiBrush, ID2D1SolidColorBrush* textBrush,
	IDWriteTextFormat* textFormat, const RECT& windowRect
) : m_context(context), m_factory(factory), m_bgBrush(bgBrush),
    m_borderBrush(borderBrush), m_uiBrush(uiBrush), m_textBrush(textBrush), m_textFormat(textFormat) {

	CreateTitleBarFigure(windowRect);
}

Renderer::~Renderer() {
	m_path.Reset();
}

void Renderer::RenderMainWindow(const RECT& rect, const GameField& gameField) const {
	RenderWindow(rect);
	RenderTitleBar(rect);
	RenderGameField(gameField);
	RenderCurrentTetramino(gameField);
	RenderGhostTetramino(gameField);
	RenderNextTetramino(gameField);
	RenderScore(gameField);
	RenderHighScore(gameField);
}

void Renderer::RenderWindow(const RECT& rect) const {
	static const float windowOff = 2.0f * UI::General::scaleFactor;
	static const float radius    = UI::MainWindow::cornerRadius;
	static const float left      = static_cast<float>(rect.left);
	static const float top       = static_cast<float>(rect.top);
	static const float right     = static_cast<float>(rect.right) - windowOff;
	static const float bottom    = static_cast<float>(rect.bottom) - windowOff;

	D2D1_ROUNDED_RECT windowRect = D2D1::RoundedRect(D2D1::RectF(left, top, right, bottom), radius, radius);

	static const float strokeWidth = UI::General::strokeWidth;
	static const float offset = strokeWidth / 2.0f;

	m_context->FillRoundedRectangle(windowRect, m_bgBrush);
	m_context->SetTransform(D2D1::Matrix3x2F::Translation(offset, offset));
	m_context->DrawRoundedRectangle(windowRect, m_borderBrush, strokeWidth);
	m_context->SetTransform(D2D1::Matrix3x2F::Identity());
}

void Renderer::RenderTitleBar(const RECT& rect) const {
	static const float right        = static_cast<float>(rect.right) - 2.0f;
	static const float bottom       = UI::MainWindow::TitleBar::tbHeight + 2.0f;
	static const float strokeWidth  = UI::General::strokeWidth;
	static const float offset       = strokeWidth / 2.0f;

	m_context->FillGeometry(m_path.Get(), m_uiBrush);
	m_context->SetTransform(D2D1::Matrix3x2F::Translation(offset, offset));
	m_context->DrawLine(D2D1::Point2F(0.0f, bottom), D2D1::Point2F(right, bottom), m_borderBrush, strokeWidth);
	m_context->SetTransform(D2D1::Matrix3x2F::Identity());
}

void Renderer::RenderGameField(const GameField& gameField) const {
	using namespace UI::MainWindow::GameField;

	static const float strokeWidth  = UI::General::strokeWidth;
	static const float offset       = strokeWidth / 2.0f;

	m_context->SetTransform(D2D1::Matrix3x2F::Translation(offset, offset));
	m_context->DrawRoundedRectangle(UI::MainWindow::d2dGameField, m_borderBrush, strokeWidth);
	m_context->SetTransform(D2D1::Matrix3x2F::Identity());

	for (int y = 0; y < UI::MainWindow::GameField::gfHeight; ++y) {
		for (int x = 0; x < UI::MainWindow::GameField::gfWidth; ++x) {
			TetraminoType type = gameField.GetGrid()[x][y];
			if (type != TetraminoType::TETRAMINO_NONE) {
				ID2D1Bitmap* bitmap = ResourceManager::GetTetraminoBitmap(type);
				D2D1_RECT_F dst = D2D1::RectF(
					offsetX + x * blockSize,
					offsetY + y * blockSize,
					offsetX + (x + 1) * blockSize,
					offsetY + (y + 1) * blockSize
				);

				m_context->DrawBitmap(bitmap, dst);
			}
		}
	}
}

void Renderer::RenderCurrentTetramino(const GameField& gameField) const {
	using namespace UI::MainWindow::GameField;

	const Tetramino& current = gameField.GetCurrentTetramino();
	ID2D1Bitmap* bitmap = ResourceManager::GetTetraminoBitmap(current.GetType());
	for (const auto& block : current.GetTetramino()) {
		vec2 pos = current.GetVisualPos() + block;

		float fx = offsetX + pos.x * blockSize;
		float fy = offsetY + pos.y * blockSize;

		fx = std::round(fx);
		fy = std::round(fy);

		D2D1_RECT_F dst = D2D1::RectF(fx, fy, fx + blockSize, fy + blockSize);
		m_context->DrawBitmap(bitmap, dst);
	}
}

void Renderer::RenderGhostTetramino(const GameField& gameField) const {
	if (!gameField.IsGhostCollide() && !gameField.GetCurrentTetramino().IsDropping()) {
		const Tetramino& ghost = gameField.GetGhostTetramino();
		ID2D1Bitmap* bitmap = ResourceManager::GetTetraminoBitmap(TetraminoType::TETRAMINO_GHOST);
		for (const auto& block : ghost.GetTetramino()) {
			vec2 pos = ghost.GetPos() + block;
			D2D1_RECT_F dst = D2D1::RectF(
				offsetX + pos.x * blockSize,
				offsetY + pos.y * blockSize,
				offsetX + (pos.x + 1) * blockSize,
				offsetY + (pos.y + 1) * blockSize
			);
			m_context->DrawBitmap(bitmap, dst);
		}
	}
}

void Renderer::RenderNextTetramino(const GameField& gameField) const {
	using namespace UI::MainWindow;
	using namespace UI::MainWindow::GameField;

	D2D1_ROUNDED_RECT& nextAreaOut  = d2dNextAreaOut;
	D2D1_ROUNDED_RECT& nextAreaIn   = d2dNextAreaIn;

	static const float strokeWidth  = UI::General::strokeWidth;
	static const float offset       = strokeWidth / 2.0f;

	m_context->FillRoundedRectangle(nextAreaOut, m_uiBrush);
	m_context->FillRoundedRectangle(nextAreaIn, m_bgBrush);
	m_context->SetTransform(D2D1::Matrix3x2F::Translation(offset, offset));
	m_context->DrawRoundedRectangle(nextAreaIn, m_borderBrush, strokeWidth);
	m_context->DrawRoundedRectangle(nextAreaOut, m_borderBrush, strokeWidth);
	m_context->SetTransform(D2D1::Matrix3x2F::Identity());

	m_context->DrawTextW(L"Next:", 5, m_textFormat, D2D1::RectF(nextAreaOut.rect.left, nextAreaOut.rect.top,
		nextAreaOut.rect.right, nextAreaIn.rect.top), m_textBrush);

	const Tetramino& next = gameField.GetNextTetramino();
	ID2D1Bitmap* bitmap = ResourceManager::GetTetraminoBitmap(next.GetType());
	for (const auto& block : next.GetTetramino()) {
		auto it = Preview::offsets.find(next.GetType());
		float offX = it->second.x;
		float offY = it->second.y;

		vec2 pos = next.GetPos() + block;
		D2D1_RECT_F dst = D2D1::RectF(
			d2dNextAreaIn.rect.left + offX + pos.x * blockSize,
			d2dNextAreaIn.rect.top + offY + pos.y * blockSize,
			d2dNextAreaIn.rect.left + offX + (pos.x + 1) * blockSize,
			d2dNextAreaIn.rect.top + offY + (pos.y + 1) * blockSize
		);

		m_context->DrawBitmap(bitmap, dst);
	}
}

void Renderer::DrawRoundedText(const D2D1_ROUNDED_RECT& rect, const std::wstring& text) const {
	static const float strokeWidth  = UI::General::strokeWidth;
	static const float offset       = strokeWidth / 2.0f;

	m_context->FillRoundedRectangle(rect, m_uiBrush);
	m_context->SetTransform(D2D1::Matrix3x2F::Translation(offset, offset));
	m_context->DrawRoundedRectangle(rect, m_borderBrush, strokeWidth);
	m_context->SetTransform(D2D1::Matrix3x2F::Identity());

	m_context->DrawTextW(text.c_str(), static_cast<uint32_t>(text.size()), m_textFormat, rect.rect, m_textBrush);
}

void Renderer::RenderScore(const GameField& gameField) const {
	DrawRoundedText(UI::MainWindow::d2dScoreRect, L"Score: " + std::to_wstring(gameField.GetScore()));
}

void Renderer::RenderHighScore(const GameField& gameField) const {
	DrawRoundedText(UI::MainWindow::d2dHighRect, L"High:  " + std::to_wstring(gameField.GetHighScore()));
}

void Renderer::UpdateAnimations(float deltaTime, GameField& gameField) {
	Tetramino& current = gameField.GetCurrentTetramino();
	Tetramino& ghost = gameField.GetGhostTetramino();
	current.UpdateAnimation(deltaTime);
	ghost.UpdateAnimation(deltaTime);
}

void Renderer::CreateTitleBarFigure(const RECT& rect) {
	static const float radius   = UI::MainWindow::cornerRadius;
	static const float left     = UI::General::strokeWidth;
	static const float top      = UI::General::strokeWidth;
	static const float right    = static_cast<float>(rect.right) - 2.0f;
	static const float bottom   = UI::MainWindow::TitleBar::tbHeight + 2.0f;

	Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;

	m_factory->CreatePathGeometry(const_cast<ID2D1PathGeometry**>(m_path.GetAddressOf()));

	m_path->Open(&sink);
	sink->BeginFigure(D2D1::Point2F(left, bottom), D2D1_FIGURE_BEGIN_FILLED);
	sink->AddLine(D2D1::Point2F(left, top + radius));
	sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(left + radius, top), D2D1::SizeF(radius, radius),
		0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->AddLine(D2D1::Point2F(right - radius, top));
	sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(right, top + radius), D2D1::SizeF(radius, radius),
		0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

	sink->AddLine(D2D1::Point2F(right, bottom));
	sink->AddLine(D2D1::Point2F(left, bottom));
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();
	sink.Reset();
}
