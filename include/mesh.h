#pragma once

#include "common.h"
#include "time_system.h"

enum class MeshPrimitiveType {
  cube,
  sphere,
  invalid,
};

class MeshVertex {
 public:
  Vec3 position;
  Vec3 normal;
  Vec2 uv;

  MeshVertex() = default;
  MeshVertex(const Vec3& position, const Vec3& normal, const Vec2& uv) : position(position), normal(normal), uv(uv) { }

  MeshVertex(const MeshVertex&) = default;
  MeshVertex(MeshVertex&&) = default;
  MeshVertex& operator=(const MeshVertex&) = default;
  MeshVertex& operator=(MeshVertex&&) = default;
  ~MeshVertex() = default;
};

class Mesh {
 public:
  static constexpr unsigned primitive_sphere_resolution = 6;

  explicit Mesh(const MeshPrimitiveType& mesh_primitive);
  explicit Mesh(std::vector<MeshVertex>&& vertices,
                GLenum buffer_data_usage_vbo = GL_STATIC_DRAW,
                bool is_bidirectional = false);
  Mesh(std::vector<MeshVertex>&& vertices, std::vector<UVec3>&& indices,
       GLenum buffer_data_usage_vbo = GL_STATIC_DRAW, GLenum buffer_data_usage_ebo = GL_STATIC_DRAW,
       bool is_bidirectional = false);

  Mesh(const Mesh&) = default;
  Mesh(Mesh&&) = default;
  Mesh& operator=(const Mesh&) = default;
  Mesh& operator=(Mesh&&) = default;
  virtual ~Mesh() = default;

  bool IsBidirectional() const;

  void DrawTriangles() const;

  virtual void CollisionHandler(std::shared_ptr<Object> obj, std::vector<Vec3> & J_mem, std::vector<Vec3> & ri_mem);
  virtual void FixedUpdate(std::vector<Vec3>& J_mem, std::vector<Vec3>& ri_mem);
  virtual void GetNP(Vec3 vertex, Vec3& P, Vec3& N);


  std::shared_ptr <Object> GetObject();

  enum class DrawMode { arrays, elements };

  std::shared_ptr<Object> object;

  DrawMode draw_mode;
  std::vector<MeshVertex> vertices;
  std::vector<UVec3> indices;
  GLenum buffer_data_usage_vbo;
  GLenum buffer_data_usage_ebo;

  bool is_bidirectional;

  Vec3 v;
  Vec3 w;

  GLuint vao;
  GLuint vbo;
  GLuint ebo;

  static constexpr unsigned simulation_steps_per_fixed_update_time = 1;
  static constexpr Float fixed_delta_time = Time::fixed_delta_time / Float(simulation_steps_per_fixed_update_time);


  void SetObject(const std::shared_ptr<Object>& new_object);

  friend class Scene;
};
