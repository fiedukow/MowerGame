#include "Lawn.h"
#include <cassert>
#include <iostream>
#include <random>

char ToChar(GroundType type) {
	switch (type) {
		case GroundType::HIGH_GRASS:
			return ',';
		case GroundType::LOW_GRASS:
			return ' ';
		case GroundType::WALL:
			return '*';
		case GroundType::STONE:
			return 'O';
		default:
			assert(false);
	}
	assert(false);
}

Lawn::Lawn(int32_t w, int32_t h, int32_t rr) : w_(w), h_(h) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, w*h-1);

	map_.reserve(w * h);
	for (int i = 0; i < w * h; ++i)
		map_.push_back(GroundType::HIGH_GRASS);
	for (int i = 0; i < rr; ++i) {
		map_.at(dis(gen)) = GroundType::STONE;
	}
}

GroundType Lawn::GroundAt(Position pos) const {
	if (pos.x >= w_ || pos.y >= h_ || pos.x < 0 || pos.y < 0) {
		// All around me are familiar walls, worn out places, worn out placeees.
		return GroundType::WALL;
	}
	return map_.at(pos.x + pos.y * w_);
}

GroundType Lawn::MowAt(Position pos) {
	assert(pos.x >= 0 && pos.y >= 0 && pos.x < w_ && pos.y < h_);
	auto& ground_at = map_.at(pos.x + pos.y * w_);
	assert(ground_at != GroundType::WALL);
	if (ground_at == GroundType::HIGH_GRASS) {
		ground_at = GroundType::LOW_GRASS;
	}
	return ground_at;
}

int32_t Lawn::GetW() const { return w_; }
int32_t Lawn::GetH() const { return h_; }

int32_t Lawn::UnmownLeft() const {
	int32_t cnt = 0;
	for (int i = 0; i < w_ * h_; ++i) {
		if (map_.at(i) == GroundType::HIGH_GRASS) {
			cnt++;
		}
	}

	return cnt;
}
