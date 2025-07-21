#include "Constants.h"

namespace Constants {
	void Init() {
		scaleFactor       = static_cast<float>(GetDpiForSystem()) / 96.0f;
		buttonTextOffsetY = 3.0f * scaleFactor;

		fontSize          = static_cast<int>(24.0f * scaleFactor);
		uiElemCornerRad   = static_cast<int>(10.0f * scaleFactor);

		titleBarHeight            = static_cast<int>(31.0f * scaleFactor);
		titleBarBtnWidth          = static_cast<int>(30.0f * scaleFactor);
		titleBarBtnHeight         = static_cast<int>(30.0f * scaleFactor);
		titleBarMinimizeBtnPosX   = static_cast<int>(465.0f * scaleFactor);
		titleBarQuitBtnPosX       = static_cast<int>(495.0f * scaleFactor);
		titleBarBtnPosY           = static_cast<int>(1.0f * scaleFactor);

		windowWidth               = static_cast<int>(532.0f * scaleFactor);
		windowHeight              = static_cast<int>(670.0f * scaleFactor);
		windowCornerRad           = static_cast<int>(16.0f * scaleFactor);

		blockSize           = static_cast<int>(30.0f * scaleFactor);
		gridOffsetX         = static_cast<int>(20.0f * scaleFactor);
		gridOffsetY         = static_cast<int>(50.0f * scaleFactor);
		uiElemWidth         = static_cast<int>(168.0f * scaleFactor);
		uiElemHeight        = static_cast<int>(44.0f * scaleFactor);
		uiElemSpacing       = static_cast<int>(15.0f * scaleFactor);
		nextWndHeight       = static_cast<int>(168.0f * scaleFactor);
		nextWndPadding      = static_cast<int>(10.0f * scaleFactor);
		nextWndTopPadding   = static_cast<int>(50.0f * scaleFactor);

		iTetraminoOffX = static_cast<int>(-77.0f * scaleFactor);
		jTetraminoOffX = static_cast<int>(-47.0f * scaleFactor);
		lTetraminoOffX = static_cast<int>(-77.0f * scaleFactor);
		oTetraminoOffX = static_cast<int>(-46.0f * scaleFactor);
		sTetraminoOffX = static_cast<int>(-62.0f * scaleFactor);
		zTetraminoOffX = static_cast<int>(-62.0f * scaleFactor);
		tTetraminoOffX = static_cast<int>(-62.0f * scaleFactor);

		iTetraminoOffY = static_cast<int>(40.0f * scaleFactor);
		jTetraminoOffY = static_cast<int>(9.0f * scaleFactor);
		lTetraminoOffY = static_cast<int>(9.0f * scaleFactor);
		oTetraminoOffY = static_cast<int>(25.0f * scaleFactor);
		sTetraminoOffY = static_cast<int>(25.0f * scaleFactor);
		zTetraminoOffY = static_cast<int>(25.0f * scaleFactor);
		tTetraminoOffY = static_cast<int>(25.0f * scaleFactor);

		mainWndBtnPosX       = gridOffsetX * 2 + gameFieldWidth * blockSize;
		mainWndPauseBtnPosY  = gridOffsetY + gameFieldHeight * blockSize - uiElemHeight * 2 - uiElemSpacing;
		mainWndQuitBtnPosY   = gridOffsetY + gameFieldHeight * blockSize - uiElemHeight;

		gameField = {
			gridOffsetX,  gridOffsetY,
			gridOffsetX + gameFieldWidth * blockSize,
			gridOffsetY + gameFieldHeight * blockSize
		};

		score = {
			gridOffsetX * 2 + gameFieldWidth * blockSize,
			gridOffsetY + nextWndHeight + uiElemSpacing,
			gridOffsetX * 2 + gameFieldWidth * blockSize + uiElemWidth,
			gridOffsetY + nextWndHeight + uiElemSpacing + uiElemHeight
		};

		highScore = {
			gridOffsetX * 2 + gameFieldWidth * blockSize,
			gridOffsetY + nextWndHeight + uiElemSpacing * 2 + uiElemHeight,
			gridOffsetX * 2 + gameFieldWidth * blockSize + uiElemWidth,
			gridOffsetY + nextWndHeight + uiElemSpacing * 2 + uiElemHeight * 2
		};

		nextWnd = {
			gridOffsetX * 2 + gameFieldWidth * blockSize, gridOffsetY,
			gridOffsetX * 2 + gameFieldWidth * blockSize + uiElemWidth,
			gridOffsetY + nextWndHeight
		};

		nextWndIn = {
			nextWnd.left + nextWndPadding, nextWnd.top + nextWndTopPadding,
			nextWnd.right - nextWndPadding, nextWnd.bottom - nextWndPadding
		};

		gameOverWndW = static_cast<int>(350.0f * scaleFactor);
		gameOverWndH = static_cast<int>(250.0f * scaleFactor);

		gameOverWndBtnWidth    = static_cast<int>(100.0f * scaleFactor);
		gameOverWndBtnHeight   = static_cast<int>(40.0f * scaleFactor);
		gameOverWndBtnSpacing  = static_cast<int>(20.0f * scaleFactor);
		gameOverWndBtnRadius   = static_cast<int>(10.0f * scaleFactor);

		totalWidth = gameOverWndBtnWidth * 2 + gameOverWndBtnSpacing;

		gameOverWndRestartButtonPosX  = static_cast<int>(57.0f * scaleFactor);
		gameOverWndQuitButtonPosX     = gameOverWndRestartButtonPosX + gameOverWndBtnWidth + gameOverWndBtnSpacing;
		gameOverWndBtnPosY            = 211 - gameOverWndBtnHeight - 20;
	}
}