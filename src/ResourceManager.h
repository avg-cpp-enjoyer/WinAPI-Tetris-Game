#pragma once

#include "TetraminoTypes.h"
#include "resource.h"
#include "Log.hpp"

#include <Windows.h>
#include <d2d1_1.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <Shlwapi.h>
#include <unordered_map>
#include <stdexcept>

#pragma comment(lib, "Shlwapi.lib")

class ResourceManager {
public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	static void Initialize(ID2D1DeviceContext* context, IWICImagingFactory* wic);
	static ID2D1Bitmap* GetTetraminoBitmap(TetraminoType type);
	static void ClearResources();
	static void Shutdown();
private:
	ResourceManager() = default;
	~ResourceManager() = default;

	static ResourceManager& GetInstance();
	void LoadResources();
	void LoadTetraminoBitmap(TetraminoType type, const wchar_t* resourceName);
private:
	void ShutdownImpl();
	ID2D1Bitmap* GetTetraminoBitmapImpl(TetraminoType type);
	void InitializeImpl(ID2D1DeviceContext* context, IWICImagingFactory* wic);
private:
	ID2D1DeviceContext* m_context = nullptr;
	IWICImagingFactory* m_wicFactory = nullptr;

	std::unordered_map<TetraminoType, Microsoft::WRL::ComPtr<ID2D1Bitmap>> m_bitmaps;
};