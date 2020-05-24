#pragma once
#include "Lawn.h"
#include "MowerOperator.h"
#include "Mower.h"

class Environment
{
public:
	Environment(Mower& mower, MowerOperator& op, Lawn& lawn, Position pos = { 0, 0 });

	void Step();
	void PrintFullState();

private:
	MowerView CurrentMowerView();
	void ExecuteStep(MowerStep step);
	void MoveMower(int32_t x, int32_t y);
	void ExecuteState();

private:
	Position mower_pos_;
	Mower& mower_;
	MowerOperator& mower_operator_;
	Lawn& lawn_;
};

