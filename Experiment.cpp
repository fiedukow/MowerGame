#include "Experiment.h"
#include <cassert>
#include <numeric>

#include "Mower.h"
#include "Environment.h"

Experiment::Experiment(ExperimentParameters params)
	: params_(std::move(params)) {}

std::vector<int> Experiment::ScoresOfEvaluations(int no_repeats) {
	std::vector<int> result;
	for (int i = 0; i < no_repeats; ++i)
		result.emplace_back(EvaluateOnce());
	return result;
}

int Experiment::EvaluateOnce() {
	Lawn lawn{ params_.width, params_.height, params_.no_rocks };
	Mower mower{ params_.mower_fuel, params_.mower_sharpness};
	auto mower_operator = params_.operator_factory();
	Environment env{ mower, *mower_operator, lawn };
	env.Evaluate(false);
	return env.Score();
}

int Experiment::AverageScore(std::vector<int> scores) {
	assert(scores.size() > 0);
	return std::accumulate(scores.begin(), scores.end(), 0) / scores.size();
}