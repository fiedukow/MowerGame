#include "Mower.h"

Mower::Mower(uint32_t fuel, uint32_t sharpness) :
	is_on_(true), fuel_(fuel), sharpness_(sharpness) {
}

void Mower::Step() {
	if (is_on_)
		--fuel_;
}
void Mower::HitRock() {
	if (is_on_)
		--sharpness_;
}

void Mower::Mowned() {
	// TODO: Maybe sharpness goes down by smaller factor?
}

void Mower::Disabled() {
	is_on_ = false;
}

void Mower::Enabled() {
	fuel_ -= 10;
	is_on_ = true;
}

bool Mower::IsOn() const {
	return is_on_;
}

bool Mower::CanOperate() const {
	return fuel_ > 0 && sharpness_ > 0;
}

uint32_t Mower::FuelLevel() const {
	return fuel_;
}

uint32_t Mower::SharpnessLevel() const {
	return sharpness_;
}