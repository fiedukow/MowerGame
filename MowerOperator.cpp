#include "MowerOperator.h"
#include <iostream>
#include <Windows.h>
#include <cassert>

MowerStep ReadStep() {
	std::cout << "=========== YOUR MOVE =============" << std::endl;
	char step;
	while (true) {
		std::cout << "What is your move? > ";
		std::cin >> step;
		switch (step) {
			case 'n': 
				return MowerStep::GO_N;
			case 's':
				return MowerStep::GO_S;
			case 'w':
				return MowerStep::GO_W;
			case 'e':
				return MowerStep::GO_E;
			case 'f':
				return MowerStep::TURN_OFF;
			case 'o':
				return MowerStep::TURN_ON;
			case 't':
				return MowerStep::STAY;
		}
		std::cout << "Invalid move. Use (n)orth, (s)outh, (w)est, (e)ast, of(f), (o)n, s(t)ay." << std::endl;
	}
}

MowerStep ManualMowerOperator::Step(const MowerView& view) {
	return ReadStep();
}

MowerStep SimpleMowerOperator::Step(const MowerView& view) {
	GroundType north = view.sensors[7];
	GroundType south = view.sensors[1];
	GroundType east = view.sensors[5];
	GroundType standing_on = view.sensors[4];
	if (current_mode_ == Mode::GOING_N && north == GroundType::WALL) {
		current_mode_ = Mode::TURNING;
	}
	if (current_mode_ == Mode::GOING_S && south == GroundType::WALL) {
		current_mode_ = Mode::TURNING;
	}
	if (current_mode_ == Mode::GOING_N && north == GroundType::STONE && view.is_on)
		return MowerStep::TURN_OFF;
	if (current_mode_ == Mode::GOING_S && south == GroundType::STONE && view.is_on)
		return MowerStep::TURN_OFF;
	if (current_mode_ == Mode::TURNING && east == GroundType::STONE && view.is_on)
		return MowerStep::TURN_OFF;

	if (standing_on == GroundType::HIGH_GRASS)
		return MowerStep::TURN_ON;

	switch (current_mode_) {
	case Mode::GOING_N: return MowerStep::GO_N;
	case Mode::GOING_S: return MowerStep::GO_S;
	case Mode::TURNING:
		current_mode_ = next_mode_;
		next_mode_ = next_mode_ == Mode::GOING_N ? Mode::GOING_S : Mode::GOING_N;
		return MowerStep::GO_E;
	}
}