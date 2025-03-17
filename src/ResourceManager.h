#pragma once

#include "resource.h"
#include "TetraminoTypes.h"

#include <Windows.h>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <gdiplus.h>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

class ResourceException : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

class ResourceManager {
public:
	ResourceManager(const ResourceManager&) = delete;

	static Gdiplus::Bitmap* GetTetraminoBitmap(TetraminoType type);
	static void ClearResources();
private:
	struct BitmapDeleter {
		void operator()(Gdiplus::Bitmap* p) const {
			delete p;
		}
	};

	ResourceManager();
	~ResourceManager() {}

	static ResourceManager& GetInstance();
	Gdiplus::Bitmap* GetTetraminoBitmapImpl(TetraminoType type) const;
	void ClearResourcesImpl();
	void LoadResources();
	void LoadTetraminoBitmap(TetraminoType type, const wchar_t* resourceName);

	using BitmapPtr = std::unique_ptr<Gdiplus::Bitmap, BitmapDeleter>;
	std::unordered_map<TetraminoType, BitmapPtr> m_bitmaps;
};

