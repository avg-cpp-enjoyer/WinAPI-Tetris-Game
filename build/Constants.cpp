#include "Constants.h"

void UI::Init() {
	using General::scaleFactor;

	General::scaleFactor = static_cast<float>(GetDpiForSystem()) / 96.0f;
	General::fontSize = 20.0f * scaleFactor;
	General::uiCornerRad = 11.0f * scaleFactor;

	MainWindow::GameField::cornerRadius = 5.0f * scaleFactor;
	MainWindow::GameField::blockSize = 30.0f *  scaleFactor;
	MainWindow::GameField::offsetX = 16.0f *    scaleFactor;
	MainWindow::GameField::offsetY = 50.0f *    scaleFactor;

	MainWindow::TitleBar::tbHeight = 30.0f * scaleFactor;
	MainWindow::TitleBar::btnWidth = 40.0f * scaleFactor;
	MainWindow::TitleBar::btnHeight = 31.0f * scaleFactor;
	MainWindow::TitleBar::minimizeBtnPosX = (518.0f - MainWindow::TitleBar::btnWidth * 2) * scaleFactor;
	MainWindow::TitleBar::quitBtnPosX = (518.0f - MainWindow::TitleBar::btnWidth) * scaleFactor;
	MainWindow::TitleBar::btnPosY = 1.0f * scaleFactor;

	MainWindow::Preview::prHeight = 168.0f * General::scaleFactor;
	MainWindow::Preview::padding = 10.0f * General::scaleFactor;
	MainWindow::Preview::topPadding = 50.0f * General::scaleFactor;

	MainWindow::Preview::offsets = {
		{ TetraminoType::TETRAMINO_I, D2D1::Point2F(-77.0f * scaleFactor, 40.0f * scaleFactor) },
		{ TetraminoType::TETRAMINO_J, D2D1::Point2F(-77.0f * scaleFactor, 9.0f * scaleFactor)  },
		{ TetraminoType::TETRAMINO_L, D2D1::Point2F(-77.0f * scaleFactor, 9.0f * scaleFactor)  },
		{ TetraminoType::TETRAMINO_O, D2D1::Point2F(-77.0f * scaleFactor, 25.0f * scaleFactor) },
		{ TetraminoType::TETRAMINO_S, D2D1::Point2F(-62.0f * scaleFactor, 25.0f * scaleFactor) },
		{ TetraminoType::TETRAMINO_Z, D2D1::Point2F(-62.0f * scaleFactor, 25.0f * scaleFactor) },
		{ TetraminoType::TETRAMINO_T, D2D1::Point2F(-62.0f * scaleFactor, 25.0f * scaleFactor) },
	};

	MainWindow::mwWidth =  static_cast<int>(520.0f * General::scaleFactor);
	MainWindow::mwHeight = static_cast<int>(670.0f * General::scaleFactor);
	MainWindow::cornerRadius = 16.0f * General::scaleFactor;
	MainWindow::uiElemWidth = 168.0f * General::scaleFactor;
	MainWindow::uiElemHeight = 44.0f * General::scaleFactor;
	MainWindow::uiElemSpacing = 16.0f * General::scaleFactor;

	MainWindow::btnPosX = MainWindow::GameField::offsetX * 2 + MainWindow::GameField::gfWidth * MainWindow::GameField::blockSize;
	MainWindow::pauseBtnPosY = MainWindow::GameField::offsetY + MainWindow::GameField::gfHeight * MainWindow::GameField::blockSize - MainWindow::uiElemHeight * 2 - MainWindow::uiElemSpacing;
	MainWindow::quitBtnPosY = MainWindow::GameField::offsetY + MainWindow::GameField::gfHeight * MainWindow::GameField::blockSize - MainWindow::uiElemHeight;
	
	MainWindow::d2dGameField = D2D1::RoundedRect(
		D2D1::RectF(
			MainWindow::GameField::offsetX,
			MainWindow::GameField::offsetY,
			MainWindow::GameField::offsetX + MainWindow::GameField::gfWidth * MainWindow::GameField::blockSize,
			MainWindow::GameField::offsetY + MainWindow::GameField::gfHeight * MainWindow::GameField::blockSize
		),
		MainWindow::GameField::cornerRadius, MainWindow::GameField::cornerRadius
	);

	MainWindow::d2dScoreRect = D2D1::RoundedRect(
		D2D1::RectF(
			MainWindow::GameField::offsetX * 2 + MainWindow::GameField::gfWidth * MainWindow::GameField::blockSize,
			MainWindow::GameField::offsetY + MainWindow::Preview::prHeight + MainWindow::uiElemSpacing,
			MainWindow::GameField::offsetX * 2 + MainWindow::GameField::gfWidth * MainWindow::GameField::blockSize + MainWindow::uiElemWidth,
			MainWindow::GameField::offsetY + MainWindow::Preview::prHeight + MainWindow::uiElemSpacing + MainWindow::uiElemHeight
		),
		General::uiCornerRad, General::uiCornerRad
	);

	MainWindow::d2dHighRect = D2D1::RoundedRect(
		D2D1::RectF(
			MainWindow::GameField::offsetX * 2 + MainWindow::GameField::gfWidth * MainWindow::GameField::blockSize,
			MainWindow::GameField::offsetY + MainWindow::Preview::prHeight + MainWindow::uiElemSpacing * 2 + MainWindow::uiElemHeight,
			MainWindow::GameField::offsetX * 2 + MainWindow::GameField::gfWidth * MainWindow::GameField::blockSize + MainWindow::uiElemWidth,
			MainWindow::GameField::offsetY + MainWindow::Preview::prHeight + MainWindow::uiElemSpacing * 2 + MainWindow::uiElemHeight * 2
		),
		General::uiCornerRad, General::uiCornerRad
	);

	MainWindow::d2dNextAreaOut = D2D1::RoundedRect(
		D2D1::RectF(
			MainWindow::GameField::offsetX * 2 + MainWindow::GameField::gfWidth * MainWindow::GameField::blockSize,
			MainWindow::GameField::offsetY,
			MainWindow::GameField::offsetX * 2 + MainWindow::GameField::gfWidth * MainWindow::GameField::blockSize + MainWindow::uiElemWidth,
			MainWindow::GameField::offsetY + MainWindow::Preview::prHeight
		),
		General::uiCornerRad, General::uiCornerRad
	);

	MainWindow::d2dNextAreaIn = D2D1::RoundedRect(
		D2D1::RectF(
			MainWindow::GameField::offsetX * 2 + MainWindow::GameField::gfWidth * MainWindow::GameField::blockSize + MainWindow::Preview::padding,
			MainWindow::GameField::offsetY + MainWindow::Preview::topPadding,
			MainWindow::GameField::offsetX * 2 + MainWindow::GameField::gfWidth * MainWindow::GameField::blockSize + MainWindow::uiElemWidth - MainWindow::Preview::padding,
			MainWindow::GameField::offsetY + MainWindow::Preview::prHeight - MainWindow::Preview::padding
		),
		General::uiCornerRad, General::uiCornerRad
	);

	MainWindow::pauseRect = D2D1::RectF(
		MainWindow::btnPosX, MainWindow::pauseBtnPosY,
		MainWindow::btnPosX + MainWindow::uiElemWidth,
		MainWindow::pauseBtnPosY + MainWindow::uiElemHeight
	);

	MainWindow::quitRect = D2D1::RectF(
		MainWindow::btnPosX, MainWindow::quitBtnPosY,
		MainWindow::btnPosX + MainWindow::uiElemWidth,
		MainWindow::quitBtnPosY + MainWindow::uiElemHeight
	);

	MainWindow::minimizeRect = D2D1::RectF(
		MainWindow::TitleBar::minimizeBtnPosX,
		MainWindow::TitleBar::btnPosY,
		MainWindow::TitleBar::minimizeBtnPosX + MainWindow::TitleBar::btnWidth,
		MainWindow::TitleBar::btnPosY + MainWindow::TitleBar::btnHeight
	);

	MainWindow::closeRect = D2D1::RectF(
		MainWindow::TitleBar::quitBtnPosX,
		MainWindow::TitleBar::btnPosY,
		MainWindow::TitleBar::quitBtnPosX + MainWindow::TitleBar::btnWidth,
		MainWindow::TitleBar::btnPosY + MainWindow::TitleBar::btnHeight
	);

	GameOver::goWidth = 350.0f * General::scaleFactor;
	GameOver::goHeight = 250.0f * General::scaleFactor;
	GameOver::btnWidth = 100.0f * General::scaleFactor;
	GameOver::btnHeight = 40.0f * General::scaleFactor;
	GameOver::btnSpacing = 20.0f * General::scaleFactor;
	GameOver::btnRadius = 10.0f * General::scaleFactor;
	GameOver::totalWidth = GameOver::btnWidth * 2 + GameOver::btnSpacing;
	GameOver::restartButtonPosX = 57.0f * General::scaleFactor;
	GameOver::quitButtonPosX = GameOver::restartButtonPosX + GameOver::btnWidth + GameOver::btnSpacing;
	GameOver::btnPosY = 191.0f - 40.0f * General::scaleFactor;

	GameOver::restartRect = D2D1::RectF(
		GameOver::restartButtonPosX,
		GameOver::btnPosY,
		GameOver::restartButtonPosX + GameOver::btnWidth,
		GameOver::btnPosY + GameOver::btnHeight
	);

	GameOver::quitRect = D2D1::RectF(
		GameOver::quitButtonPosX,
		GameOver::btnPosY,
		GameOver::quitButtonPosX + GameOver::btnWidth,
		GameOver::btnPosY + GameOver::btnHeight
	);
}