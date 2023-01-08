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
  float restitution;

  /// interfaces
  virtual void CollisionHandler(std::shared_ptr<Object> obj, std::vector<Vec3> & J_mem, std::vector<Vec3> & ri_mem);
  //virtual void CollisionDetect(std::shared_ptr<Mesh> mesh) override;
  virtual void FixedUpdate(std::vector<Vec3>& J_mem, std::vector<Vec3>& ri_mem) override;
  virtual void GetNP(Vec3 vertex, Vec3& P, Vec3& N);


 private:

  std::vector<Vec3> world_velocities;
  std::vector<Vec3> world_accelerations;
};
