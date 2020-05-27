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
	experiment.EvaluateOnce(true);
	//std::cout << "Avg. score is: " << Experiment::AverageScore(experiment.ScoresOfEvaluations(1000)) << std::endl;
	return 0;
}