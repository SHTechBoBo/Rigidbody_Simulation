#include "geometry.h"

#include <utility>
#include <iostream>
#include <algorithm>
#include <random>
#include <time.h>

TriangleMesh::TriangleMesh(std::vector<Vec3f> vertices, std::vector<Vec3f> normals,
                           std::vector<int> v_index, std::vector<int> n_index) :
    vertices(std::move(vertices)),
    normals(std::move(normals)),
    v_indices(std::move(v_index)),
    n_indices(std::move(n_index)) {}

