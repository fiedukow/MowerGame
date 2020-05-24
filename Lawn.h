#pragma once
#include <vector>

struct Position {
	int32_t x;
	int32_t y;
};

enum class GroundType {
	WALL,
	STONE,
	HIGH_GRASS,
	LOW_GRASS
};

char ToChar(GroundType type);

class Lawn
{
public:
	Lawn(int32_t w, int32_t h, int32_t rr);
	GroundType GroundAt(Position pos) const;
	GroundType MowAt(Position pos);

	int32_t GetW() const;
	int32_t GetH() const;
	int32_t UnmownLeft() const;

private:
	int32_t w_;
	int32_t h_;
	std::vector<GroundType> map_;
};

