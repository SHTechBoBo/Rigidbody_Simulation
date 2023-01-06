#pragma once

#include "mesh.h"
#include "time_system.h"

class Ball : public Mesh {
 public:

  /// constructor

  Ball();

  Ball(const Ball&) = default;
  Ball(Ball&&) = default;
  Ball& operator=(const Ball&) = default;
  Ball& operator=(Ball&&) = default;
  virtual ~Ball() override = default;



  /// interfaces

  virtual void FixedUpdate() override;

 private:
  static constexpr unsigned simulation_steps_per_fixed_update_time = 4;
  static constexpr Float fixed_delta_time = Time::fixed_delta_time / Float(simulation_steps_per_fixed_update_time);

  std::vector<Vec3> world_velocities;
  std::vector<Vec3> world_accelerations;
};