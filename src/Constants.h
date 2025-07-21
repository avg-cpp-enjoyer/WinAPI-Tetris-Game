#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <shellscalingapi.h>
#include <unordered_map>

#include "TetraminoTypes.h"

#pragma comment(lib, "Shcore.lib")

namespace Constants {
	void Init();

	inline float scaleFactor = 1.0f;
	inline float buttonTextOffsetY;

	inline constexpr int gameFieldWidth = 10;
	inline constexpr int gameFieldHeight = 20;
	inline float gridCornerRadius;

	inline float fontSize;
	inline float uiElemCornerRad;

	inline constexpr float strokeWidth = 1.0f;

	inline const D2D1::ColorF bgColor(30.0f / 255, 30.0f / 255, 30.0f / 255, 1.0f);
	inline const D2D1::ColorF uiColor(40.0f / 255, 40.0f / 255, 40.0f / 255, 1.0f);
	inline const D2D1::ColorF borderColor(80.0f / 255, 80.0f / 255, 80.0f / 255, 1.0f);
	inline const D2D1::ColorF btnClrDefault(40.0f / 255, 40.0f / 255, 40.0f / 255, 1.0f);
	inline const D2D1::ColorF btnClrHovered(50.0f / 255, 50.0f / 255, 50.0f / 255, 1.0f);
	inline const D2D1::ColorF btnClrClicked(40.0f / 255, 40.0f / 255, 40.0f / 255, 1.0f);
	inline const D2D1::ColorF uiTextColor(128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f);

	inline float titleBarHeight;
	inline float titleBarBtnWidth;
	inline float titleBarBtnHeight;
	inline float titleBarMinimizeBtnPosX;
	inline float titleBarQuitBtnPosX;
	inline float titleBarBtnPosY;

	inline int windowWidth;
	inline int windowHeight;
	inline float windowCornerRad;

	inline float blockSize;
	inline float gridOffsetX;
	inline float gridOffsetY;
	inline float uiElemWidth;
	inline float uiElemHeight;
	inline float uiElemSpacing;
	inline float nextWndHeight;
	inline float nextWndPadding;
	inline float nextWndTopPadding;

	inline std::unordered_map<TetraminoType, D2D1_POINT_2F> offsets;

	inline float iTetraminoOffX, jTetraminoOffX, lTetraminoOffX, oTetraminoOffX;
	inline float sTetraminoOffX, zTetraminoOffX, tTetraminoOffX;
	inline float iTetraminoOffY, jTetraminoOffY, lTetraminoOffY, oTetraminoOffY;
	inline float sTetraminoOffY, zTetraminoOffY, tTetraminoOffY;

	inline float mainWndBtnPosX;
	inline float mainWndPauseBtnPosY;
	inline float mainWndQuitBtnPosY;

	inline RECT gameField;
	inline RECT score;
	inline RECT highScore;
	inline RECT nextAreaOut;
	inline RECT nextAreaIn;

	inline D2D1_ROUNDED_RECT d2dNextAreaOut;
	inline D2D1_ROUNDED_RECT d2dNextAreaIn;
	inline D2D1_ROUNDED_RECT d2dGameField;
	inline D2D1_ROUNDED_RECT d2dScoreRect;
	inline D2D1_ROUNDED_RECT d2dHighRect;

	inline D2D1_RECT_F pauseRect;
	inline D2D1_RECT_F quitRect;
	inline D2D1_RECT_F closeRect;
	inline D2D1_RECT_F minimizeRect;
	inline D2D1_RECT_F gameOverRestartRect;
	inline D2D1_RECT_F gameOverQuitRect;

	inline float gameOverWndWidth;
	inline float gameOverWndHeight;
	inline float gameOverWndBtnWidth;
	inline float gameOverWndBtnHeight;
	inline float gameOverWndBtnSpacing;
	inline float gameOverWndBtnRadius;
	inline float totalWidth;
	inline float gameOverWndRestartButtonPosX;
	inline float gameOverWndQuitButtonPosX;
	inline float gameOverWndBtnPosY;
}