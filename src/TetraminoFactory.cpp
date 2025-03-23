#include "TetraminoFactory.h"

std::unique_ptr<Tetramino> TetraminoFactory::CreateTetramino(TetraminoType type) {
	switch (type) {
	case TetraminoType::TETRAMINO_I: return std::make_unique<Tetramino_I>();
	case TetraminoType::TETRAMINO_J: return std::make_unique<Tetramino_J>();
	case TetraminoType::TETRAMINO_L: return std::make_unique<Tetramino_L>();
	case TetraminoType::TETRAMINO_O: return std::make_unique<Tetramino_O>();
	case TetraminoType::TETRAMINO_S: return std::make_unique<Tetramino_S>();
	case TetraminoType::TETRAMINO_Z: return std::make_unique<Tetramino_Z>();
	case TetraminoType::TETRAMINO_T: return std::make_unique<Tetramino_T>();
	}

	return nullptr;
}