#include "MowerOperator.h"
#include <iostream>
#include <Windows.h>
#include <cassert>

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
			std::cout << ToCharWithCol(view.sensors[x + y*size_of_sensors]);
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
	PrintMowerView(view);
	return ReadStep();
}

MowerStep SimpleMowerOperator::Step(const MowerView& view) {
	PrintMowerView(view);
	Sleep(100);
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