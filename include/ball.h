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

  float mass;
  Mat3 I_ref;
  Vec3 v;
  Vec3 w;

  /// interfaces
  void CollisionHandler(Vec3 P, Vec3 N);
  virtual void CollisionDetect(std::shared_ptr<Mesh> mesh) override;
  virtual void FixedUpdate() override;

 private:
  static constexpr unsigned simulation_steps_per_fixed_update_time = 1;
  static constexpr Float fixed_delta_time = Time::fixed_delta_time / Float(simulation_steps_per_fixed_update_time);

  std::vector<Vec3> world_velocities;
  std::vector<Vec3> world_accelerations;
};
