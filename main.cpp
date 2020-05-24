#include "Lawn.h"
#include "Environment.h"
#include "Mower.h"
#include "MowerOperator.h"
#include <iostream>

int main() {
	const int W = 20;
	const int H = 10;
	const int ROCKS = W*H*0.10;
	Lawn lawn{ W, H, ROCKS };
	Mower mower{ static_cast<uint32_t>(W*H*2), 3 };
	SimpleMowerOperator manual_operator{};
	Environment env{ mower, manual_operator, lawn };
	while (mower.CanOperate() && lawn.UnmownLeft() > 0) {
		env.PrintFullState();
		env.Step();
	}

	env.PrintFullState();
	std::cout << "===========================================" << std::endl;
	if (lawn.UnmownLeft() == 0)
		std::cout << "============ GAME OVER - NOTHING TO MOW!" << std::endl;
	else if (!mower.CanOperate())
		std::cout << "============ GAME OVER - MOWER IS DEAD!" << std::endl;
	std::cout << "===========================================" << std::endl;

	std::cout << "===========================================" << std::endl;
	std::cout << "============ SCORE: " << env.Score() << std::endl;
	std::cout << "===========================================" << std::endl;
	return 0;
}