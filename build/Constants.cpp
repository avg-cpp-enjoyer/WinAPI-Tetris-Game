#include "Constants.h"

namespace Constants {
	void Init() {
		scaleFactor               = static_cast<float>(GetDpiForSystem()) / 96.0f;
		buttonTextOffsetY         = 3.0f   * scaleFactor;
						     
		gridCornerRadius          = 5.0f   * scaleFactor;

		fontSize                  = 20.0f  * scaleFactor;
		uiElemCornerRad           = 11.0f  * scaleFactor;

		titleBarHeight            = 31.0f  * scaleFactor;
		titleBarBtnWidth          = 40.0f  * scaleFactor;
		titleBarBtnHeight         = 31.0f  * scaleFactor;
		titleBarMinimizeBtnPosX   = 440.0f * scaleFactor;
		titleBarQuitBtnPosX       = 480.0f * scaleFactor;
		titleBarBtnPosY           = 1.6f   * scaleFactor;

		windowWidth               = static_cast<int>(521.0f * scaleFactor);
		windowHeight              = static_cast<int>(668.0f * scaleFactor);
		windowCornerRad           = 14.0f  * scaleFactor;

		blockSize                 = 30.0f  * scaleFactor;
		gridOffsetX               = 16.0f  * scaleFactor;
		gridOffsetY               = 50.0f  * scaleFactor;
		uiElemWidth               = 168.0f * scaleFactor;
		uiElemHeight              = 44.0f  * scaleFactor;
		uiElemSpacing             = 15.0f  * scaleFactor;
		nextWndHeight             = 168.0f * scaleFactor;
		nextWndPadding            = 10.0f  * scaleFactor;
		nextWndTopPadding         = 50.0f  * scaleFactor;

		offsets = {
			{ TetraminoType::TETRAMINO_I, D2D1::Point2F(-77.0f * scaleFactor, 40.0f * scaleFactor) },
			{ TetraminoType::TETRAMINO_J, D2D1::Point2F(-77.0f * scaleFactor,  9.0f * scaleFactor) },
			{ TetraminoType::TETRAMINO_L, D2D1::Point2F(-77.0f * scaleFactor,  9.0f * scaleFactor) },
			{ TetraminoType::TETRAMINO_O, D2D1::Point2F(-77.0f * scaleFactor, 25.0f * scaleFactor) },
			{ TetraminoType::TETRAMINO_S, D2D1::Point2F(-62.0f * scaleFactor, 25.0f * scaleFactor) },
			{ TetraminoType::TETRAMINO_Z, D2D1::Point2F(-62.0f * scaleFactor, 25.0f * scaleFactor) },
			{ TetraminoType::TETRAMINO_T, D2D1::Point2F(-62.0f * scaleFactor, 25.0f * scaleFactor) },
		};

		mainWndBtnPosX       = static_cast<float>(gridOffsetX * 2 + gameFieldWidth * blockSize);
		mainWndPauseBtnPosY  = static_cast<float>(gridOffsetY + gameFieldHeight * blockSize - uiElemHeight * 2 - uiElemSpacing);
		mainWndQuitBtnPosY   = static_cast<float>(gridOffsetY + gameFieldHeight * blockSize - uiElemHeight);

		gameField = {
			static_cast<long>(gridOffsetX),
			static_cast<long>(gridOffsetY),
			static_cast<long>(gridOffsetX + gameFieldWidth * blockSize),
			static_cast<long>(gridOffsetY + gameFieldHeight * blockSize)
		};

		score = {
			static_cast<long>(gridOffsetX * 2 + gameFieldWidth * blockSize),
			static_cast<long>(gridOffsetY + nextWndHeight + uiElemSpacing),
			static_cast<long>(gridOffsetX * 2 + gameFieldWidth * blockSize + uiElemWidth),
			static_cast<long>(gridOffsetY + nextWndHeight + uiElemSpacing + uiElemHeight)
		};

		highScore = {
			static_cast<long>(gridOffsetX * 2 + gameFieldWidth * blockSize),
			static_cast<long>(gridOffsetY + nextWndHeight + uiElemSpacing * 2 + uiElemHeight),
			static_cast<long>(gridOffsetX * 2 + gameFieldWidth * blockSize + uiElemWidth),
			static_cast<long>(gridOffsetY + nextWndHeight + uiElemSpacing * 2 + uiElemHeight * 2)
		};

		nextAreaOut = {
			static_cast<long>(gridOffsetX * 2 + gameFieldWidth * blockSize), 
			static_cast<long>(gridOffsetY),
			static_cast<long>(gridOffsetX * 2 + gameFieldWidth * blockSize + uiElemWidth),
			static_cast<long>(gridOffsetY + nextWndHeight)
		};

		nextAreaIn = {
			static_cast<long>(nextAreaOut.left + nextWndPadding), 
			static_cast<long>(nextAreaOut.top + nextWndTopPadding),
			static_cast<long>(nextAreaOut.right - nextWndPadding), 
			static_cast<long>(nextAreaOut.bottom - nextWndPadding)
		};

		d2dNextAreaOut = D2D1::RoundedRect(D2D1::RectF(
			static_cast<float>(nextAreaOut.left), 
			static_cast<float>(nextAreaOut.top), 
			static_cast<float>(nextAreaOut.right), 
			static_cast<float>(nextAreaOut.bottom)), 
			uiElemCornerRad, uiElemCornerRad);

		d2dNextAreaIn = D2D1::RoundedRect(D2D1::RectF(
			static_cast<float>(nextAreaIn.left), 
			static_cast<float>(nextAreaIn.top), 
			static_cast<float>(nextAreaIn.right), 
			static_cast<float>(nextAreaIn.bottom)), 
			uiElemCornerRad - 3.0f * scaleFactor, 
			uiElemCornerRad - 3.0f * scaleFactor);

		d2dGameField = D2D1::RoundedRect(D2D1::RectF(
			gridOffsetX, gridOffsetY, 
			gridOffsetX + gameFieldWidth  * blockSize, 
			gridOffsetY + gameFieldHeight * blockSize),
			gridCornerRadius, gridCornerRadius);

		d2dScoreRect = D2D1::RoundedRect(D2D1::RectF(
			static_cast<float>(score.left),  
			static_cast<float>(score.top),
			static_cast<float>(score.right), 
			static_cast<float>(score.bottom)
		), uiElemCornerRad, uiElemCornerRad);

		d2dHighRect = D2D1::RoundedRect(D2D1::RectF(
			static_cast<float>(highScore.left),  
			static_cast<float>(highScore.top),
			static_cast<float>(highScore.right), 
			static_cast<float>(highScore.bottom)
		), uiElemCornerRad, uiElemCornerRad);

		pauseRect = D2D1::RectF(mainWndBtnPosX, mainWndPauseBtnPosY, mainWndBtnPosX + uiElemWidth, mainWndPauseBtnPosY + uiElemHeight);
		quitRect  = D2D1::RectF(mainWndBtnPosX, mainWndQuitBtnPosY, mainWndBtnPosX + uiElemWidth, mainWndQuitBtnPosY + uiElemHeight);

		closeRect = D2D1::RectF(titleBarQuitBtnPosX, titleBarBtnPosY,
			titleBarQuitBtnPosX + titleBarBtnWidth, 
			titleBarBtnPosY + titleBarBtnHeight);

		minimizeRect = D2D1::RectF(titleBarMinimizeBtnPosX, titleBarBtnPosY, 
			titleBarMinimizeBtnPosX + titleBarBtnWidth,
			titleBarBtnPosY + titleBarBtnHeight);

		gameOverWndWidth              = 350.0f * scaleFactor;
		gameOverWndHeight             = 250.0f * scaleFactor;
		gameOverWndBtnWidth           = 100.0f * scaleFactor;
		gameOverWndBtnHeight          = 40.0f  * scaleFactor;
		gameOverWndBtnSpacing         = 20.0f  * scaleFactor;
		gameOverWndBtnRadius          = 10.0f  * scaleFactor;
		gameOverWndRestartButtonPosX  = 60.0f  * scaleFactor;
		gameOverWndQuitButtonPosX     = gameOverWndRestartButtonPosX + gameOverWndBtnWidth + gameOverWndBtnSpacing;
		gameOverWndBtnPosY            = 205 - gameOverWndBtnHeight;

		gameOverRestartRect = D2D1::RectF(gameOverWndRestartButtonPosX, gameOverWndBtnPosY,
			gameOverWndRestartButtonPosX + gameOverWndBtnWidth,
			gameOverWndBtnPosY + gameOverWndBtnHeight
		);

		gameOverQuitRect = D2D1::RectF(gameOverWndQuitButtonPosX, gameOverWndBtnPosY,
			gameOverWndQuitButtonPosX + gameOverWndBtnWidth,
			gameOverWndBtnPosY + gameOverWndBtnHeight
		);
	}
}