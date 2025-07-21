#pragma once

#include "Base.h"
#include "GameField.h"
#include "ResourceManager.h"
#include "Button.h"
#include "TitleButton.h"
#include "Constants.h"
#include "GameOverWindow.h"
#include "Renderer.hpp"
#include "Log.hpp"
#include "JobSystem.hpp"

#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <d2d1_2.h>
#include <dcomp.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <comdef.h>
#include <memory>
#include <chrono>
#include <mutex>
#include <deque>
#include <shared_mutex>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dcomp.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "Windowscodecs.lib")

enum class Command {
	MoveLeft, MoveRight, MoveDown, Rotate, HardDrop, Pause
};

class TetrisWindow final : public Base<TetrisWindow> {
public:
	TetrisWindow() = default;
	~TetrisWindow() override {}

	const wchar_t* ClassName() const override;
	intptr_t HandleMessage(uint32_t msg, uintptr_t wParam, intptr_t lParam) override;
	void Exec(int cmdShow);
private:
	void InitD3D();
	void InitD2D();
	void InitDComp();
	void InitText();
	void InitBrushes();
	void Cleanup();
	void RenderLoop();
	void LogicLoop();
	bool GravityStep();
	void ExecuteCommand(Command cmd);
	void CreateButtons();
	void CreateTitleButtons();
	intptr_t OnNcHitTest(intptr_t lParam) const;
	void OnKeyDown(uintptr_t key);
	bool CreateGameOverWindow();
	void RestartCallback();
	void RenderFrame();
	void PauseGame();
	void OnCreate();
	void OnDestroy();
	void OnGameOver();
private:
	GameField                  m_gameField;
	std::unique_ptr<Renderer>  m_renderer;
	static constexpr uint32_t  WM_APP_GAMEOVER = WM_APP + 1;

	std::thread                m_renderThread;
	std::thread                m_logicThread;
	mutable std::shared_mutex  m_gameFieldMutex;
	std::mutex                 m_cmdMutex;
	std::mutex                 m_restartMutex;
	std::condition_variable    m_cmdCV;
	std::condition_variable    m_restartCV;
	std::deque<Command>        m_commands;
	std::atomic<bool>          m_renderRunning{ false };
	std::atomic<bool>          m_logicRunning{ false };
	std::atomic<bool>          m_isPaused{ false };
	std::atomic<bool>          m_waitingForRestart{ false };
	std::atomic<bool>          m_needsRedraw{ false };
	std::atomic<bool>          m_hardDropDone{ false };
	float m_logicAccumulator = 0.0f;

	Microsoft::WRL::ComPtr<ID3D11Device>          m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>   m_d3dContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>       m_swapChain;
	Microsoft::WRL::ComPtr<IDXGIDevice>           m_dxgiDevice;

	Microsoft::WRL::ComPtr<ID2D1Factory1>         m_d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device>           m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1>   m_d2dContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1>          m_d2dTarget;

	Microsoft::WRL::ComPtr<IDCompositionDevice>   m_dcompDevice;
	Microsoft::WRL::ComPtr<IDCompositionTarget>   m_dcompTarget;
	Microsoft::WRL::ComPtr<IDCompositionVisual>   m_dcompVisual;

	Microsoft::WRL::ComPtr<IWICImagingFactory>    m_wicFactory;
	Microsoft::WRL::ComPtr<IDWriteFactory>        m_writeFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat>     m_textFormat;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>  m_bgBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>  m_uiBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>  m_borderBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>  m_textBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>  m_gridBrush;

	std::unique_ptr<Button> m_pauseButton;
	std::unique_ptr<Button> m_quitButton;
	std::unique_ptr<Button> m_minimizeButton;
	std::unique_ptr<Button> m_closeButton;

	std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double, std::nano>> m_nextTick;
};