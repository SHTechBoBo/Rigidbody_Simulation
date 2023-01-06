#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "core.h"

#include <vector>
#include <optional>

struct Triangle
{
    Vec3f p1{ 0, 0, 0 };
    Vec3f p2{ 0, 0, 0 };
    Vec3f p3{ 0, 0, 0 };
};

class TriangleMesh {
 public:
  TriangleMesh() = default;
  TriangleMesh(std::vector<Vec3f> vertices,
               std::vector<Vec3f> normals,
               std::vector<int> v_index,
               std::vector<int> n_index);

  std::vector<Vec3f> vertices;
  std::vector<Vec3f> normals;
  std::vector<int> v_indices;
  std::vector<int> n_indices;
};

#endif // GEOMETRY_H_
