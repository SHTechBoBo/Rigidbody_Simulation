#include "mesh.h"
#include "object.h"
#include "transform.h"

Object::Object(std::shared_ptr<Mesh> mesh,
               std::shared_ptr<Shader> shader) :
  mesh(std::move(mesh)),
  shader(std::move(shader)),
  transform(std::make_unique<Transform>()),
  color(0, 0, 0) {
}

Object::Object(std::shared_ptr<Mesh> mesh,
               std::shared_ptr<Shader> shader,
               const Transform& transform) :
  mesh(std::move(mesh)),
  shader(std::move(shader)),
  transform(std::make_unique<Transform>(transform)),
  color(0, 0, 0){
}

void Object::CollisionHandler(std::shared_ptr<Object> obj, std::vector<Vec3> & J_mem, std::vector<Vec3> & ri_mem)
{
    if (mesh && obj->mesh) {
        mesh->CollisionHandler(obj, J_mem, ri_mem);
    }
}

void Object::FixedUpdate(std::vector<Vec3> & J_mem, std::vector<Vec3> & ri_mem) const {
    if (mesh) {
        mesh->FixedUpdate(J_mem, ri_mem);
    }
}

void Object::SetTag(std::string t) {
    tag = t;
    if (tag == "Ball") {
        aabb = AABB(transform->position);
    }

}

std::string Object::GetTag() {
    return tag;
}


void Object::DrawAABB() {

    std::vector<Vec3> points;
    points.push_back(aabb.low_bnd);
    points.push_back(aabb.upper_bnd);

    unsigned int vao_, vbo_;

    // TODO
}