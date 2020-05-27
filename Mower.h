#pragma once
#include <cstdint>

class Mower
{
public:
	Mower(int fuel, int sharpness);

	void Step();
	void HitRock();
	void Mowned();
	void Disabled();
	void Enabled();

	bool IsOn() const;
	bool CanOperate() const;
	int FuelLevel() const;
	int SharpnessLevel() const;

private:
	bool is_on_;
	int fuel_;
	int sharpness_;
};

