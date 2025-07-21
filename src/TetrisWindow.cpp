#include "TetrisWindow.h"

const wchar_t* TetrisWindow::ClassName() const {
	return L"TetrisWindowClass";
}

intptr_t TetrisWindow::HandleMessage(uint32_t msg, uintptr_t wParam, intptr_t lParam) {
	switch (msg) {
	case WM_NCCALCSIZE:
		return 0;
	case WM_NCHITTEST:
		return OnNcHitTest(lParam);
	case WM_CREATE:
		OnCreate();
		return 0;
	case WM_KEYDOWN:
		OnKeyDown(wParam);
		return 0;
	case WM_APP_GAMEOVER:
		OnGameOver();
		return 0;
	case WM_DESTROY:
		OnDestroy();
	}
	return DefWindowProc(m_window, msg, wParam, lParam);
}

void TetrisWindow::Exec(int cmdShow) {
	ShowWindow(m_window, cmdShow);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void TetrisWindow::InitD3D() {
	uint32_t flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HR_LOG(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags,
		nullptr, 0, D3D11_SDK_VERSION, &m_d3dDevice, nullptr, &m_d3dContext));

	HR_LOG(m_d3dDevice.As(&m_dxgiDevice));

	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	HR_LOG(m_dxgiDevice->GetAdapter(&adapter));
	Microsoft::WRL::ComPtr<IDXGIFactory2> factory2;
	HR_LOG(adapter->GetParent(IID_PPV_ARGS(&factory2)));

	RECT rc; 
	GetClientRect(m_window, &rc);

	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width        = rc.right;
	scDesc.Height       = rc.bottom;
	scDesc.Format       = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.SampleDesc   = { 1, 0 };
	scDesc.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount  = 2;
	scDesc.SwapEffect   = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	scDesc.AlphaMode    = DXGI_ALPHA_MODE_PREMULTIPLIED;
	scDesc.Scaling      = DXGI_SCALING_STRETCH;

	HR_LOG(factory2->CreateSwapChainForComposition(m_d3dDevice.Get(), &scDesc, nullptr, &m_swapChain));
}

void TetrisWindow::InitD2D() {
	D2D1_FACTORY_OPTIONS opts = {};
#ifdef _DEBUG
	opts.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	HR_LOG(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory1), &opts,
		reinterpret_cast<void**>(m_d2dFactory.GetAddressOf())));

	HR_LOG(m_d2dFactory->CreateDevice(m_dxgiDevice.Get(), &m_d2dDevice));

	Microsoft::WRL::ComPtr<ID2D1DeviceContext> baseContext;
	HR_LOG(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &baseContext));
	HR_LOG(baseContext.As(&m_d2dContext));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> back;
	HR_LOG(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&back)));
	Microsoft::WRL::ComPtr<IDXGISurface> surface;
	HR_LOG(back.As(&surface));

	D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

	HR_LOG(m_d2dContext->CreateBitmapFromDxgiSurface(surface.Get(), bitmapProps, &m_d2dTarget));
	m_d2dContext->SetTarget(m_d2dTarget.Get());
}

void TetrisWindow::InitDComp() {
	HR_LOG(DCompositionCreateDevice(m_dxgiDevice.Get(), IID_PPV_ARGS(&m_dcompDevice)));
	HR_LOG(m_dcompDevice->CreateTargetForHwnd(m_window, true, &m_dcompTarget));
	HR_LOG(m_dcompDevice->CreateVisual(&m_dcompVisual));
	HR_LOG(m_dcompVisual->SetContent(m_swapChain.Get()));
	HR_LOG(m_dcompTarget->SetRoot(m_dcompVisual.Get()));

	Microsoft::WRL::ComPtr<IDCompositionVisual2> visual2;
	if (SUCCEEDED(m_dcompVisual.As(&visual2))) {
		visual2->SetBitmapInterpolationMode(DCOMPOSITION_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	}

	HR_LOG(m_dcompDevice->Commit());
}

void TetrisWindow::InitText() {
	HR_LOG(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_wicFactory)));

	HR_LOG(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), 
		reinterpret_cast<IUnknown**>(m_writeFactory.GetAddressOf())));

	HR_LOG(m_writeFactory->CreateTextFormat(L"Bahnschrift", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, Constants::fontSize, L"", &m_textFormat));

	m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void TetrisWindow::InitBrushes() {
	HR_LOG(m_d2dContext->CreateSolidColorBrush(Constants::bgColor, &m_bgBrush));
	HR_LOG(m_d2dContext->CreateSolidColorBrush(Constants::uiColor, &m_uiBrush));
	HR_LOG(m_d2dContext->CreateSolidColorBrush(Constants::borderColor, &m_borderBrush));
	HR_LOG(m_d2dContext->CreateSolidColorBrush(Constants::uiTextColor, &m_textBrush));
	HR_LOG(m_d2dContext->CreateSolidColorBrush(Constants::borderColor, &m_gridBrush));
}

void TetrisWindow::Cleanup() {
	m_swapChain   .Reset();
	m_d2dTarget   .Reset();
	m_d2dContext  .Reset();
	m_d2dDevice   .Reset();
	m_dxgiDevice  .Reset();
	m_d3dContext  .Reset();
	m_d3dDevice   .Reset();
	m_d2dFactory  .Reset();
	m_wicFactory  .Reset();
	m_writeFactory.Reset();
	m_textFormat  .Reset();
	m_bgBrush     .Reset();
	m_uiBrush     .Reset();
	m_borderBrush .Reset();
	m_textBrush   .Reset();
	m_gridBrush   .Reset();
}

void TetrisWindow::RenderLoop() {
	constexpr double targetFPS = 144.0;
	const auto frameDuration = std::chrono::duration<double>(1.0 / targetFPS);
	auto lastTime = std::chrono::steady_clock::now();
	auto nextFrame = lastTime + frameDuration;

	while (m_renderRunning.load(std::memory_order_relaxed)) {
		std::this_thread::sleep_until(nextFrame);
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<double> delta = now - lastTime;
		lastTime = now;
		nextFrame += frameDuration;
		{
			std::shared_lock lk(m_gameFieldMutex);
			m_renderer->UpdateAnimations(static_cast<float>(delta.count()), m_gameField);
			auto& cur = m_gameField.GetCurrentTetramino();
			if (!cur.IsAnimating() && cur.IsDropping()) {
				m_hardDropDone.store(true, std::memory_order_relaxed);
				m_cmdCV.notify_one();
			}
		}
		RenderFrame();
	}
}

void TetrisWindow::LogicLoop() {
	static constexpr auto tickInterval = std::chrono::duration<double>(0.5);
	auto now = std::chrono::steady_clock::now();
	m_nextTick = now + tickInterval;

	std::unique_lock<std::mutex> lock(m_cmdMutex);
	while (m_logicRunning.load(std::memory_order_relaxed)) {
		m_cmdCV.wait_until(lock, m_nextTick, [&] {
			return !m_logicRunning.load() || !m_commands.empty() || m_hardDropDone.load();
		});

		if (!m_logicRunning.load()) {
			break;
		}

		std::deque<Command> commandBuffer;
		commandBuffer.swap(m_commands);
		lock.unlock();
		for (auto cmd : commandBuffer) {
			ExecuteCommand(cmd);
		}
		lock.lock();

		if (m_isPaused.load(std::memory_order_relaxed)) {
			m_nextTick = std::chrono::steady_clock::now() + tickInterval;
			continue;
		}

		if (m_gameField.IsGameOver()) {
			PostMessage(m_window, WM_APP_GAMEOVER, 0, 0);
			m_isPaused.store(true);
			m_nextTick = std::chrono::steady_clock::now() + tickInterval;
			continue;
		}

		if (m_hardDropDone.exchange(false)) {
			{
				std::shared_lock dropLock(m_gameFieldMutex);
				m_gameField.LockTetramino();
			}
			m_nextTick = std::chrono::steady_clock::now();
			m_needsRedraw.store(true, std::memory_order_relaxed);
			continue;
		}

		now = std::chrono::steady_clock::now();
		if (now >= m_nextTick) {
			m_nextTick = now + tickInterval;
			GravityStep();
		}

		m_needsRedraw.store(true, std::memory_order_relaxed);
	}
}

bool TetrisWindow::GravityStep() {
	if (m_gameField.GetCurrentTetramino().IsDropping()) {
		return false;
	}

	m_gameField.Update();
	return true;
}

void TetrisWindow::ExecuteCommand(Command cmd) {
	if (m_gameField.GetCurrentTetramino().IsDropping()) {
		return;
	}

	std::unique_lock lock(m_gameFieldMutex);

	switch (cmd) {
	case Command::MoveLeft:  
		m_gameField.MoveCurrent(Direction::DIRECTION_LEFT); 
		break;
	case Command::MoveRight: 
		m_gameField.MoveCurrent(Direction::DIRECTION_RIGHT); 
		break;
	case Command::MoveDown:  
		m_gameField.MoveCurrent(Direction::DIRECTION_DOWN); 
		break;
	case Command::Rotate:  
		m_gameField.RotateCurrent(); 
		break;
	case Command::HardDrop:
		m_gameField.HardDrop();
		break;
	case Command::Pause:     
		PauseGame(); 
		break;
	}

	m_gameField.UpdateGhostPos();
}

void TetrisWindow::CreateButtons() {
	m_pauseButton = std::make_unique<Button>(m_window, L"Pause", Constants::pauseRect, Constants::uiElemCornerRad,
		false, Constants::uiTextColor, Constants::borderColor, Constants::btnClrDefault,
		Constants::btnClrClicked, Constants::btnClrHovered, m_textFormat, m_d2dContext.Get());

	m_quitButton = std::make_unique<Button>(m_window, L"Quit", Constants::quitRect, Constants::uiElemCornerRad,
		false, Constants::uiTextColor, Constants::borderColor, Constants::btnClrDefault,
		Constants::btnClrClicked, Constants::btnClrHovered, m_textFormat, m_d2dContext.Get());

	m_pauseButton->SetOnClick([this]() {
		PauseGame();
		std::lock_guard<std::mutex> lg(m_cmdMutex);
		m_commands.clear();
	});

	m_quitButton->SetOnClick([this]() {
		PostMessage(m_window, WM_DESTROY, 0, 0);
	});
}

void TetrisWindow::CreateTitleButtons() {
	m_closeButton = std::make_unique<TitleButton>(m_window, L"✕", Constants::closeRect, Constants::windowCornerRad,
		true, Constants::uiTextColor, Constants::uiColor, Constants::btnClrDefault,
		Constants::btnClrClicked, Constants::btnClrHovered, m_textFormat, m_d2dContext.Get());

	m_minimizeButton = std::make_unique<Button>(m_window, L"⤵", Constants::minimizeRect, 0.0f,
		true, Constants::uiTextColor, Constants::uiColor, Constants::btnClrDefault,
		Constants::btnClrClicked, Constants::btnClrHovered, m_textFormat, m_d2dContext.Get());

	m_closeButton->SetOnClick([this]() {
		PostMessage(m_window, WM_CLOSE, 0, 0);
	});

	m_minimizeButton->SetOnClick([this]() {
		ShowWindow(m_window, SW_MINIMIZE);
	});
}

intptr_t TetrisWindow::OnNcHitTest(intptr_t lParam) const {
	POINT pt = { LOWORD(lParam), HIWORD(lParam) };
	ScreenToClient(m_window, &pt);

	if (pt.y < Constants::titleBarHeight) {
		return HTCAPTION;
	}

	return DefWindowProc(m_window, WM_NCHITTEST, 0, lParam);
}

void TetrisWindow::OnKeyDown(uintptr_t key) {
	std::lock_guard lock(m_cmdMutex);

	switch (key) {
	case VK_LEFT:  
		m_commands.push_back(Command::MoveLeft); break;
	case VK_RIGHT: 
		m_commands.push_back(Command::MoveRight);break;
	case VK_DOWN:  
		m_commands.push_back(Command::MoveDown); break;
	case VK_UP:    
		m_commands.push_back(Command::Rotate); break;
	case VK_SPACE:
		m_commands.push_back(Command::HardDrop); break;
	}

	m_cmdCV.notify_one();
}

bool TetrisWindow::CreateGameOverWindow() {
	RECT rcMain;
	GetWindowRect(m_window, &rcMain);

	const int x = rcMain.left + (rcMain.right - rcMain.left - static_cast<int>(Constants::gameOverWndWidth)) / 2;
	const int y = rcMain.top + (rcMain.bottom - rcMain.top - static_cast<int>(Constants::gameOverWndHeight)) / 2;

	GameOverWindow gameOver(m_gameField.GetScore(), GameField::GetHighScore());
	gameOver.Create(L"Game Over", WS_POPUP | WS_VISIBLE, WS_EX_LAYERED,
		x, y, static_cast<int>(Constants::gameOverWndWidth), static_cast<int>(Constants::gameOverWndHeight));

	EnableWindow(m_window, false);

	ShowWindow(gameOver.Window(), SW_SHOW);
	UpdateWindow(gameOver.Window());

	MSG msg;
	while (IsWindowVisible(gameOver.Window()) && GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	EnableWindow(m_window, true);
	SetForegroundWindow(m_window);

	return gameOver.ShouldRestart();
}

void TetrisWindow::RestartCallback() {
	m_waitingForRestart.store(false, std::memory_order_relaxed);
	m_restartCV.notify_one();
	m_waitingForRestart = true;
}

void TetrisWindow::RenderFrame() {
	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(D2D1::ColorF(0, 0, 0, 0));

	RECT rc; 
	GetClientRect(m_window, &rc);
	m_renderer->RenderMainWindow(rc, m_gameField);

	m_pauseButton->Draw(); 
	m_quitButton->Draw();
	m_minimizeButton->Draw(); 
	m_closeButton->Draw();

	m_d2dContext->EndDraw();
	m_swapChain->Present(1, 0);
}

void TetrisWindow::PauseGame() {
	bool newState = !m_isPaused.load();
	m_isPaused.store(newState);

	if (newState) {
		m_pauseButton->SetText(L"Continue");
	} else {
		m_pauseButton->SetText(L"Pause");
	}
}

void TetrisWindow::OnCreate() {
	InitD3D();
	InitD2D();
	InitDComp();
	InitText();
	InitBrushes();

	RECT rect;
	GetClientRect(m_window, &rect);

	m_renderer = std::make_unique<Renderer>(m_d2dContext.Get(), m_d2dFactory.Get(), m_bgBrush.Get(),
		m_borderBrush.Get(), m_uiBrush.Get(), m_textBrush.Get(), m_textFormat.Get(), rect);

	ResourceManager::Initialize(m_d2dContext.Get(), m_wicFactory.Get());
	CreateButtons();
	CreateTitleButtons();

	m_logicRunning.store(true, std::memory_order_relaxed);
	m_logicThread = std::thread(&TetrisWindow::LogicLoop, this);

	m_renderRunning.store(true, std::memory_order_relaxed);
	m_renderThread = std::thread(&TetrisWindow::RenderLoop, this);
}

void TetrisWindow::OnDestroy() {
	m_logicRunning.store(false, std::memory_order_relaxed);
	m_cmdCV.notify_all();

	if (m_logicThread.joinable()) {
		m_logicThread.join();
	}

	m_renderRunning.store(false, std::memory_order_relaxed);
	if (m_renderThread.joinable()) {
		m_renderThread.join();
	}

	m_pauseButton.reset();
	m_quitButton.reset();
	m_minimizeButton.reset();
	m_closeButton.reset();
	m_renderer.reset();

	ResourceManager::Shutdown();
	Cleanup();
	PostQuitMessage(0);
}

void TetrisWindow::OnGameOver() {
	m_isPaused.store(true);
	m_cmdCV.notify_all();

	if (!CreateGameOverWindow()) {
		PostMessage(m_window, WM_DESTROY, 0, 0);
	}

	{
		std::lock_guard lock(m_gameFieldMutex);
		m_gameField.Reset();
	}

	m_isPaused.store(false);
	m_cmdCV.notify_all();
	m_nextTick = std::chrono::steady_clock::now();
}
