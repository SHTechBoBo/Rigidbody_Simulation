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

	virtual void FixedUpdate(std::vector<Vec3>& J_mem, std::vector<Vec3>& ri_mem) override;

private:

	std::vector<Vec3> world_velocities;
	std::vector<Vec3> world_accelerations;
};
