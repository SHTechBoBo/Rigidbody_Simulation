#pragma once

#include "common.h"

struct AABB {
	// ×óÏÂ ÓÒÉÏ
	Vec3 low_bnd;
	Vec3 upper_bnd;

	AABB() : low_bnd(0, 0, 0), upper_bnd(0, 0, 0) {};

	AABB(Vec3 center) {
		low_bnd = center - Vec3(0.5f, 0, 0) - Vec3(0, 0.5f, 0) - Vec3(0, 0, 0.5f);
		upper_bnd = center + Vec3(0.5f, 0, 0) + Vec3(0, 0.5f, 0) + Vec3(0, 0, 0.5f);
	};

	Vec3 getCenter() {
		return (low_bnd + upper_bnd) / 2.0f;
	}

	bool intersect(struct AABB& aabb) {
		return 
			((low_bnd.x >= aabb.low_bnd.x && low_bnd.x <= aabb.upper_bnd.x) || (aabb.low_bnd.x >= low_bnd.x && aabb.low_bnd.x <= upper_bnd.x)) &&
			((low_bnd.y >= aabb.low_bnd.y && low_bnd.y <= aabb.upper_bnd.y) || (aabb.low_bnd.y >= low_bnd.y && aabb.low_bnd.y <= upper_bnd.y)) &&
			((low_bnd.z >= aabb.low_bnd.z && low_bnd.z <= aabb.upper_bnd.z) || (aabb.low_bnd.z >= low_bnd.z && aabb.low_bnd.z <= upper_bnd.z));
	}

	bool containPoint(const Vec3& point) const
	{
		if (point.x < low_bnd.x) return false;
		if (point.y < low_bnd.y) return false;
		if (point.z < low_bnd.z) return false;
		if (point.x > upper_bnd.x) return false;
		if (point.y > upper_bnd.y) return false;
		if (point.z > upper_bnd.z) return false;
		return true;
	}
};



class Object final {
 public:
  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Shader> shader;
  std::unique_ptr<Transform> transform;
  Vec3 color;

  struct AABB aabb;

  Object(std::shared_ptr<Mesh> mesh,
         std::shared_ptr<Shader> shader);
  Object(std::shared_ptr<Mesh> mesh,
         std::shared_ptr<Shader> shader,
         const Transform& transform);

  Object(const Object&) = delete;
  Object(Object&&) = default;
  Object& operator=(const Object&) = delete;
  Object& operator=(Object&&) = default;
  ~Object() = default;

  void CollisionHandler(std::shared_ptr<Object> obj, std::vector<Vec3> & J_mem, std::vector<Vec3> & ri_mem);
  void FixedUpdate(std::vector<Vec3>& J_mem, std::vector<Vec3>& ri_mem) const;
  
  void DrawAABB();
  void SetTag(std::string t);
  std::string GetTag();

private:
  std::string tag;
};
