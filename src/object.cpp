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
  color(0, 0, 0) {
}

void Object::CollisionDetect(std::shared_ptr<Object> obj) {
    if (mesh && obj->mesh) {
        mesh->CollisionDetect(obj->mesh);
    }

 
}


void Object::FixedUpdate() const {
    if (mesh) {
        mesh->FixedUpdate();
    }
}

void Object::SetTag(std::string t) {
    tag = t;
}

std::string Object::GetTag() {
    return tag;
}