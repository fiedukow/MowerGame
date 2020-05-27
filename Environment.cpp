#include "Environment.h"
#include <algorithm> 
#include <iostream>
#include <cassert>
#include <Windows.h>

const int32_t kSensorRange = 1;

char ToCharWithCol(GroundType type) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (type) {
	case GroundType::HIGH_GRASS:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN);
		return ',';
	case GroundType::LOW_GRASS:
		SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
		return ' ';
	case GroundType::WALL:
		SetConsoleTextAttribute(hConsole, 8);
		return '*';
	case GroundType::STONE:
		SetConsoleTextAttribute(hConsole, 15 | BACKGROUND_GREEN);
		return 'O';
	default:
		assert(false);
	}
	assert(false);
}


void PrintMowerView(const MowerView& view) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "=========== MOWER VIEW ============" << std::endl;
	int32_t size_of_sensors = static_cast<int32_t>(sqrt(view.sensors.size()));
	for (int y = size_of_sensors - 1; y >= 0; --y) {
		for (int x = 0; x < size_of_sensors; ++x) {
			std::cout << ToCharWithCol(view.sensors[x + y * size_of_sensors]);
		}
		std::cout << std::endl;
	}
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "Fuel: ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	std::cout << view.fuel_level;
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "; Sharpness: ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cout << view.sharpness_level;
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << ";" << std::endl;
	if (view.is_on)
		std::cout << "Mower goes brum brum..." << std::endl;
	else {
		SetConsoleTextAttribute(hConsole, 8);
		std::cout << "The sound of silence..." << std::endl;
	}
	SetConsoleTextAttribute(hConsole, 15);
}

Environment::Environment(Mower& mower, MowerOperator& op, Lawn& lawn, Position pos) :
	mower_(mower), mower_operator_(op), lawn_(lawn), mower_pos_(pos), time_(0) {
	ExecuteState();
}

void Environment::Evaluate(bool print_steps) {
	while (mower_.CanOperate() && lawn_.UnmownLeft() > 0) {
		if (print_steps)
			PrintFullState();
		Step();
	}
}

void Environment::Step() {
	const auto step = mower_operator_.Step(CurrentMowerView());
	ExecuteStep(step);
	ExecuteState();
	time_++;
}

int32_t Environment::Score() const {
	return lawn_.GetW() * lawn_.GetH() - 6*lawn_.UnmownLeft() + 3 * mower_.FuelLevel() + 60 * mower_.SharpnessLevel() - time_;
}

void Environment::PrintFullState() {
	system("CLS");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "           SCORE: " << Score() << std::endl;
	std::cout << "========== GLOBAL VIEW ============" << std::endl;
	for (int y = lawn_.GetH() - 1; y >= 0; --y) {
		for (int x = 0; x < lawn_.GetW(); ++x) {
			if (mower_pos_.x == x && mower_pos_.y == y) {
				SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
				std::cout << "M";
			}
			else {
				std::cout << ToCharWithCol(lawn_.GroundAt({ x,y }));
			}
		}
		std::cout << std::endl;
	}
	SetConsoleTextAttribute(hConsole, 15);
	PrintMowerView(CurrentMowerView());
	SetConsoleTextAttribute(hConsole, 15);
}

void Environment::ExecuteStep(MowerStep step) {
	switch (step) {
	case MowerStep::GO_N:
		MoveMower(0, 1);
		break;
	case MowerStep::GO_S:
		MoveMower(0, -1);
		break;
	case MowerStep::GO_W:
		MoveMower(-1, 0);
		break;
	case MowerStep::GO_E:
		MoveMower(1, 0);
		break;
	case MowerStep::TURN_OFF:
		mower_.Disabled();
		break;
	case MowerStep::TURN_ON:
		mower_.Enabled();
		break;
	case MowerStep::STAY:
		break;
	}

	mower_.Step();
}

void Environment::MoveMower(int32_t x, int32_t y) {
	int32_t new_x = mower_pos_.x + x;
	new_x = max(0, min(static_cast<int32_t>(lawn_.GetW() - 1), new_x));
	int32_t new_y = mower_pos_.y + y;
	new_y = max(0, min(static_cast<int32_t>(lawn_.GetH() - 1), new_y));
	auto new_pos = Position{ new_x, new_y };
	if (lawn_.GroundAt(new_pos) == GroundType::WALL)
		return;
	mower_pos_ = new_pos;
}

MowerView Environment::CurrentMowerView() {
	std::vector<GroundType> sensors;
	for (int y = mower_pos_.y - kSensorRange; y <= mower_pos_.y + kSensorRange; ++y) {
		for (int x = mower_pos_.x - kSensorRange; x <= mower_pos_.x + kSensorRange; ++x) {
			sensors.push_back(lawn_.GroundAt({ x, y }));
		}
	}
	return MowerView{
		sensors,
		mower_.FuelLevel(),
		mower_.SharpnessLevel(),
		mower_.IsOn()
	};
}

void Environment::ExecuteState() {
	const GroundType ground_under_mowner = lawn_.GroundAt(mower_pos_);
	switch (ground_under_mowner)
	{
	case GroundType::HIGH_GRASS:
		if (mower_.IsOn()) {
			lawn_.MowAt(mower_pos_);
			mower_.Mowned();
		}
		break;
	case GroundType::LOW_GRASS:
		break;
	case GroundType::WALL:
		assert(false);
		break;
	case GroundType::STONE:
		if (mower_.IsOn()) {
			lawn_.MowAt(mower_pos_);
			mower_.HitRock();
		}
		break;
	}
}
