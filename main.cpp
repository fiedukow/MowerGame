#include "MowerOperator.h"
#include "Experiment.h"
#include <iostream>
#include <memory>

int main() {
	const int W = 20;
	const int H = 10;
	const int ROCKS = W*H*0.10;
	const int FUEL = W * H * 2;
	const int SHARPNESS = 3;
	Experiment experiment{
		ExperimentParameters{
			W, H, ROCKS, FUEL, SHARPNESS,
			[]() { return std::make_unique<SimpleMowerOperator>(); }} };
	std::cout << "Avg. score is: " << Experiment::AverageScore(experiment.ScoresOfEvaluations(1000)) << std::endl;

	//env.PrintFullState();
	//std::cout << "===========================================" << std::endl;
	//if (lawn.UnmownLeft() == 0)
	//	std::cout << "============ GAME OVER - NOTHING TO MOW!" << std::endl;
	//else if (!mower.CanOperate())
	//	std::cout << "============ GAME OVER - MOWER IS DEAD!" << std::endl;
	//std::cout << "===========================================" << std::endl;

	//std::cout << "===========================================" << std::endl;
	//std::cout << "============ SCORE: " << env.Score() << std::endl;
	//std::cout << "===========================================" << std::endl;
	return 0;
}