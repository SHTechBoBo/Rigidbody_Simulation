#include "scene.h"

Scene::Scene(Float camera_fov_y) :
  camera(camera_fov_y),
  light_position(0, 0, 0),
  light_color(0, 0, 0) {
}

std::shared_ptr<Object> Scene::AddObject(const std::shared_ptr<Mesh>& mesh,
                                         const std::shared_ptr<Shader>& shader) {
  objects.emplace_back(
    std::make_shared<Object>(mesh, shader));
  mesh->SetObject(objects.back());
  return objects.back();
}

std::shared_ptr<Object> Scene::AddObject(const std::shared_ptr<Mesh>& mesh,
                                         const std::shared_ptr<Shader>& shader,
                                         const Transform& transform) {
  objects.emplace_back(
    std::make_shared<Object>(mesh, shader, transform));
  mesh->SetObject(objects.back());
  return objects.back();
}

void Scene::Update() {
  camera.Update();
}

//关键是这个
/*
检测所有碰撞
为所有obj计算受力（区分有无碰撞）
迭代所有obj状态
*/
void Scene::FixedUpdate() {
    // 计算碰撞
    for (auto& object_1 : objects) {
        for (auto& object_2 : objects) {
            if (object_1 != object_2) {
                object_1->CollisionDetect(object_2);
            }
        }
    }


    // 更新位置
    for (auto& object : objects) {
        object->FixedUpdate();
    }
}

void Scene::RenderUpdate() {
  for (const auto& object : objects) {
    if (object->mesh && object->shader && object->transform) {
      object->shader->Set("model", object->transform->ModelMat());
      object->shader->Set("view", camera.LookAtMat());
      object->shader->Set("projection", camera.PerspectiveMat());
      object->shader->Set("object_color", object->color);
      object->shader->Set("light_position", light_position);
      object->shader->Set("light_color", light_color);
      object->shader->Set("camera_position", camera.transform.position);
      object->shader->Set("is_bidirectional", object->mesh->IsBidirectional());
      object->shader->Use();
      object->mesh->DrawTriangles();
    }
  }
}
