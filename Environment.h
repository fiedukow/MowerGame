#pragma once
#include "Lawn.h"
#include "MowerOperator.h"
#include "Mower.h"

struct ScoreBase {
	int init_fuel;
	int init_sharpness;
	int init_grass;
};

class Environment
{
public:
	Environment(Mower& mower, MowerOperator& op, Lawn& lawn, Position pos = { 0, 0 });

	void Evaluate(bool print_steps);
	void Step();
	void PrintFullState();
	int Score() const;

private:
	MowerView CurrentMowerView();
	void ExecuteStep(MowerStep step);
	void MoveMower(int32_t x, int32_t y);
	void ExecuteState();

private:
	const ScoreBase score_base_;
	Position mower_pos_;
	Mower& mower_;
	MowerOperator& mower_operator_;
	Lawn& lawn_;
	int32_t time_;
};

