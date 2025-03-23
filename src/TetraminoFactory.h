#include "TetraminoTypes.h"
#include "Tetramino.h"

#include <memory>

class TetraminoFactory {
public:
	TetraminoFactory() = delete;
	static std::unique_ptr<Tetramino> CreateTetramino(TetraminoType type);
};