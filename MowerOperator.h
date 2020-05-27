#pragma once

#include <vector>
#include "Lawn.h"

struct MowerView {
	std::vector<GroundType> sensors;
	int fuel_level;
	int sharpness_level;
	bool is_on;
};

enum class MowerStep {
	GO_N,
	GO_S,
	GO_W,
	GO_E,
	STAY,
	TURN_OFF,
	TURN_ON
};

class MowerOperator
{
public:
	virtual MowerStep Step(const MowerView& view) = 0;
};

class ManualMowerOperator : public MowerOperator
{
public:
	MowerStep Step(const MowerView& view) override;
};


class SimpleMowerOperator : public MowerOperator
{
	enum class Mode {
		GOING_N,
		TURNING,
		GOING_S
	};
public:
	MowerStep Step(const MowerView& view) override;

private:
	Mode current_mode_ = Mode::GOING_N;
	Mode next_mode_ = Mode::GOING_S;
};
