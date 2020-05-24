#pragma once
#include <cstdint>

class Mower
{
public:
	Mower(uint32_t fuel, uint32_t sharpness);

	void Step();
	void HitRock();
	void Mowned();
	void Disabled();
	void Enabled();

	bool IsOn() const;
	bool CanOperate() const;
	uint32_t FuelLevel() const;
	uint32_t SharpnessLevel() const;

private:
	bool is_on_;
	uint32_t fuel_;
	uint32_t sharpness_;
};

