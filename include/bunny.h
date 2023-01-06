#pragma once

#include "mesh.h"
#include "time_system.h"

class Bunny : public Mesh {
public:

	/// constructor

	Bunny();

	Bunny(const Bunny&) = default;
	Bunny(Bunny&&) = default;
	Bunny& operator=(const Bunny&) = default;
	Bunny& operator=(Bunny&&) = default;
	virtual ~Bunny() override = default;



	/// interfaces

	virtual void FixedUpdate() override;

private:
	static constexpr unsigned simulation_steps_per_fixed_update_time = 4;
	static constexpr Float fixed_delta_time = Time::fixed_delta_time / Float(simulation_steps_per_fixed_update_time);

	std::vector<Vec3> world_velocities;
	std::vector<Vec3> world_accelerations;
};
