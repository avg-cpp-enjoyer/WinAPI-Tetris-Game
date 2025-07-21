#include "TetraminoTypes.h"
#include "Tetramino.h"

#include <random>

class TetraminoManager {
public:
	TetraminoManager(const TetraminoManager&) = delete;
	TetraminoManager& operator=(const TetraminoManager&) = delete;

	static Tetramino CreateTetramino(TetraminoType type);
	static Tetramino CreateRandomTetramino();
private:
	TetraminoManager();
	static TetraminoManager& GetInstance();

	static Tetramino CreateTetraminoImpl(TetraminoType type);
	Tetramino CreateRandomTetraminoImpl();
private:
	std::uniform_int_distribution<int> m_distribution{ 1, 7 };
	std::mt19937 m_rng;
};