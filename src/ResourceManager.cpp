#include "ResourceManager.h"
#include <string>

Gdiplus::Bitmap* ResourceManager::GetTetraminoBitmap(TetraminoType type) {
	return GetInstance().GetTetraminoBitmapImpl(type);
}

void ResourceManager::ClearResources() {
	GetInstance().ClearResourcesImpl();
}

ResourceManager::ResourceManager() {
	m_bitmaps.emplace(TetraminoType::TETRAMINO_I, BitmapPtr(nullptr, BitmapDeleter()));
	m_bitmaps.emplace(TetraminoType::TETRAMINO_J, BitmapPtr(nullptr, BitmapDeleter()));
	m_bitmaps.emplace(TetraminoType::TETRAMINO_L, BitmapPtr(nullptr, BitmapDeleter()));
	m_bitmaps.emplace(TetraminoType::TETRAMINO_O, BitmapPtr(nullptr, BitmapDeleter()));
	m_bitmaps.emplace(TetraminoType::TETRAMINO_S, BitmapPtr(nullptr, BitmapDeleter()));
	m_bitmaps.emplace(TetraminoType::TETRAMINO_Z, BitmapPtr(nullptr, BitmapDeleter()));
	m_bitmaps.emplace(TetraminoType::TETRAMINO_T, BitmapPtr(nullptr, BitmapDeleter()));
    
	LoadResources();
}

ResourceManager& ResourceManager::GetInstance() {
	static ResourceManager instance;
	return instance;
}

void ResourceManager::LoadResources() {
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_I, MAKEINTRESOURCE(IDB_TETRAMINO_I));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_J, MAKEINTRESOURCE(IDB_TETRAMINO_J));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_L, MAKEINTRESOURCE(IDB_TETRAMINO_L));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_O, MAKEINTRESOURCE(IDB_TETRAMINO_O));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_S, MAKEINTRESOURCE(IDB_TETRAMINO_S));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_Z, MAKEINTRESOURCE(IDB_TETRAMINO_Z));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_T, MAKEINTRESOURCE(IDB_TETRAMINO_T));
}

void ResourceManager::LoadTetraminoBitmap(TetraminoType type, const wchar_t* resourceName) {
	HRSRC hRes = FindResource(nullptr, resourceName, L"PNG");
	if (!hRes) {
		throw ResourceException("Resource not found: " + std::to_string(static_cast<int>(type)));
	}

	HGLOBAL hData = LoadResource(nullptr, hRes);
	if (!hData) {
		throw ResourceException("Failed to load resource: " + std::to_string(GetLastError()));
	}

	void* pData = LockResource(hData);
	if (!pData) {
		throw ResourceException("Failed to lock resource");
	}

	DWORD size = SizeofResource(nullptr, hRes);
	if (size == 0) {
		throw ResourceException("Empty resource data");
	}

	IStream* pStream = SHCreateMemStream(static_cast<const BYTE*>(pData), size);
	if (!pStream) {
		throw ResourceException("Failed to create memory stream");
	}

	Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromStream(pStream);
	pStream->Release();

	if (bitmap && bitmap->GetLastStatus() == Gdiplus::Ok) {
		m_bitmaps[type].reset(bitmap);
	} else {
		throw ResourceException("Failed to create bitmap from resource");
	}
}

void ResourceManager::ClearResourcesImpl() {
	m_bitmaps.clear();
}

Gdiplus::Bitmap* ResourceManager::GetTetraminoBitmapImpl(TetraminoType type) const {
	auto it = m_bitmaps.find(type);
	if (it != m_bitmaps.end() && it->second) {
		return it->second.get();
	}

	return nullptr;
}
