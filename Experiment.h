#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "MowerOperator.h"

struct ExperimentParameters {
	int width;
	int height;
	int no_rocks;
	uint32_t mower_fuel;
	uint32_t mower_sharpness;
	std::function<std::unique_ptr<MowerOperator>()> operator_factory;
};

class Experiment
{
public:
	explicit Experiment(ExperimentParameters params);
	std::vector<int> ScoresOfEvaluations(int no_repeats);
	int EvaluateOnce();

	static int AverageScore(std::vector<int> scores);

private:
	const ExperimentParameters params_;
};

