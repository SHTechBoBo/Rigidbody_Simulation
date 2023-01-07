#pragma once

#include "mesh.h"
#include "time_system.h"

class Wall : public Mesh {
public:

	/// constructor

	Wall(Vec3 scale, Vec3 rotate);

	Wall(const Wall&) = default;
	Wall(Wall&&) = default;
	Wall& operator=(const Wall&) = default;
	Wall& operator=(Wall&&) = default;
	virtual ~Wall() override = default;
	virtual void GetNP(Vec3 vertex, Vec3& P, Vec3& N);




	/// interfaces

	virtual void FixedUpdate() override;

private:
	static constexpr unsigned simulation_steps_per_fixed_update_time = 4;
	static constexpr Float fixed_delta_time = Time::fixed_delta_time / Float(simulation_steps_per_fixed_update_time);

	std::vector<Vec3> world_velocities;
	std::vector<Vec3> world_accelerations;
};
