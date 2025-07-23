#pragma once

#include <d2d1.h>
#include <shellscalingapi.h>
#include <unordered_map>

#include "TetraminoTypes.h"

#pragma comment(lib, "Shcore.lib")

namespace UI {
	void Init();

	namespace General {
		inline float scaleFactor = 1.0f;

		inline float fontSize;
		inline float uiCornerRad;

		inline constexpr float strokeWidth = 1.0f;

		inline const D2D1::ColorF bgColor(30.0f / 255, 30.0f / 255, 30.0f / 255, 1.0f);
		inline const D2D1::ColorF uiColor(40.0f / 255, 40.0f / 255, 40.0f / 255, 1.0f);
		inline const D2D1::ColorF borderColor(80.0f / 255, 80.0f / 255, 80.0f / 255, 1.0f);
		inline const D2D1::ColorF btnClrDefault(40.0f / 255, 40.0f / 255, 40.0f / 255, 1.0f);
		inline const D2D1::ColorF btnClrHovered(50.0f / 255, 50.0f / 255, 50.0f / 255, 1.0f);
		inline const D2D1::ColorF btnClrClicked(40.0f / 255, 40.0f / 255, 40.0f / 255, 1.0f);
		inline const D2D1::ColorF textColor(128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f);
	}

	namespace MainWindow {
		namespace GameField {
			inline constexpr int gfWidth = 10;
			inline constexpr int gfHeight = 20;
			inline float cornerRadius;
			inline float blockSize;
			inline float offsetX;
			inline float offsetY;
		}

		namespace TitleBar {
			inline float tbHeight;
			inline float btnWidth;
			inline float btnHeight;
			inline float minimizeBtnPosX;
			inline float quitBtnPosX;
			inline float btnPosY;
		}

		namespace Preview {
			inline float prHeight;
			inline float padding;
			inline float topPadding;
			inline std::unordered_map<TetraminoType, D2D1_POINT_2F> offsets;
		}

		inline int mwWidth;
		inline int mwHeight;
		inline float cornerRadius;
		inline float uiElemWidth;
		inline float uiElemHeight;
		inline float uiElemSpacing;
		inline float btnPosX;
		inline float pauseBtnPosY;
		inline float quitBtnPosY;

		inline D2D1_ROUNDED_RECT d2dNextAreaOut;
		inline D2D1_ROUNDED_RECT d2dNextAreaIn;
		inline D2D1_ROUNDED_RECT d2dGameField;
		inline D2D1_ROUNDED_RECT d2dScoreRect;
		inline D2D1_ROUNDED_RECT d2dHighRect;

		inline D2D1_RECT_F pauseRect;
		inline D2D1_RECT_F quitRect;
		inline D2D1_RECT_F closeRect;
		inline D2D1_RECT_F minimizeRect;
	}

	namespace GameOver {
		inline float goWidth;
		inline float goHeight;
		inline float btnWidth;
		inline float btnHeight;
		inline float btnSpacing;
		inline float btnRadius;
		inline float totalWidth;
		inline float restartButtonPosX;
		inline float quitButtonPosX;
		inline float btnPosY;

		inline D2D1_RECT_F restartRect;
		inline D2D1_RECT_F quitRect;
	}
}

namespace Render {
	inline constexpr double targetFPS = 144.0;
}