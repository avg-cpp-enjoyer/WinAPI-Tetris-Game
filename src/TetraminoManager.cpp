#include "TetraminoManager.h"

TetraminoManager::TetraminoManager() {
	m_rng.seed(std::random_device{}());
}

Tetramino TetraminoManager::CreateTetramino(TetraminoType type){
	return GetInstance().CreateTetraminoImpl(type);
}

Tetramino TetraminoManager::CreateRandomTetramino() {
	return GetInstance().CreateRandomTetraminoImpl();
}

TetraminoManager& TetraminoManager::GetInstance() {
	static TetraminoManager instance;
	return instance;
}

Tetramino TetraminoManager::CreateTetraminoImpl(TetraminoType type) {
	switch (type) {
	case TetraminoType::TETRAMINO_I: return Tetramino_I();
	case TetraminoType::TETRAMINO_J: return Tetramino_J();
	case TetraminoType::TETRAMINO_L: return Tetramino_L();
	case TetraminoType::TETRAMINO_O: return Tetramino_O();
	case TetraminoType::TETRAMINO_S: return Tetramino_S();
	case TetraminoType::TETRAMINO_Z: return Tetramino_Z();
	case TetraminoType::TETRAMINO_T: return Tetramino_T();
	}

	return Tetramino_Z();
}

Tetramino TetraminoManager::CreateRandomTetraminoImpl() {
	return CreateTetramino(static_cast<TetraminoType>(m_distribution(m_rng)));
}
