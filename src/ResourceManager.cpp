#include "ResourceManager.h"

void ResourceManager::Initialize(ID2D1DeviceContext* context, IWICImagingFactory* wic) {
	GetInstance().InitializeImpl(context, wic);
}

ID2D1Bitmap* ResourceManager::GetTetraminoBitmap(TetraminoType type) {
	return GetInstance().GetTetraminoBitmapImpl(type);
}

void ResourceManager::ClearResources() {
	GetInstance().m_bitmaps.clear();
}

void ResourceManager::Shutdown() {
	GetInstance().ShutdownImpl();
}

ResourceManager& ResourceManager::GetInstance() {
	static ResourceManager instance;
	return instance;
}

void ResourceManager::LoadResources() {
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_I,     MAKEINTRESOURCEW(IDB_TETRAMINO_I));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_J,     MAKEINTRESOURCEW(IDB_TETRAMINO_J));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_L,     MAKEINTRESOURCEW(IDB_TETRAMINO_L));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_O,     MAKEINTRESOURCEW(IDB_TETRAMINO_O));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_S,     MAKEINTRESOURCEW(IDB_TETRAMINO_S));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_Z,     MAKEINTRESOURCEW(IDB_TETRAMINO_Z));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_T,     MAKEINTRESOURCEW(IDB_TETRAMINO_T));
	LoadTetraminoBitmap(TetraminoType::TETRAMINO_GHOST, MAKEINTRESOURCEW(IDB_TETRAMINO_GHOST));
}

void ResourceManager::LoadTetraminoBitmap(TetraminoType type, const wchar_t* resourceName) {
	HRSRC resource = FindResourceW(nullptr, resourceName, L"PNG");   
	if (!resource) {
		throw std::runtime_error("Resource not found for TetraminoType");
	}

	HGLOBAL data = LoadResource(nullptr, resource);
	if (!data) {
		throw std::runtime_error("Failed to LoadResource");
	}

	void* dataPtr = LockResource(data);
	DWORD size = SizeofResource(nullptr, resource);

	Microsoft::WRL::ComPtr<IStream> stream;
	stream.Attach(SHCreateMemStream(reinterpret_cast<const BYTE*>(dataPtr), size));
	if (!stream) {
		throw std::runtime_error("Failed to create memory stream");
	}

	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
	HR_THROW(m_wicFactory->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnLoad, &decoder), 
		"WIC create decoder failed");

	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	HR_THROW(decoder->GetFrame(0, &frame), "WIC get frame failed");

	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	HR_THROW(m_wicFactory->CreateFormatConverter(&converter), "WIC create converter failed");

	HR_THROW(converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
		nullptr, 0.f, WICBitmapPaletteTypeMedianCut), "WIC format converter init failed");

	Microsoft::WRL::ComPtr<ID2D1Bitmap> d2dBitmap;
	HR_THROW(m_context->CreateBitmapFromWicBitmap(converter.Get(), nullptr, &d2dBitmap), "CreateBitmapFromWicBitmap failed");

	m_bitmaps[type] = std::move(d2dBitmap);
}

void ResourceManager::ShutdownImpl() {
	m_bitmaps.clear();
	m_context = nullptr;
	m_wicFactory = nullptr;
}

ID2D1Bitmap* ResourceManager::GetTetraminoBitmapImpl(TetraminoType type) {
	auto it = m_bitmaps.find(type);
	return (it != m_bitmaps.end()) ? it->second.Get() : nullptr;
}

void ResourceManager::InitializeImpl(ID2D1DeviceContext* context, IWICImagingFactory* wic) {
	m_context = context;
	m_wicFactory = wic;
	LoadResources();
}
