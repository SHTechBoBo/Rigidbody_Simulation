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
    std::vector<std::vector<Vec3>> J_mem, ri_mem;
    for (int i = 0; i < objects.size(); i++) {
        J_mem.push_back(std::vector<Vec3>());
        ri_mem.push_back(std::vector<Vec3>());
        objects[i]->mesh->v[1] -= 9.8f * objects[i]->mesh->fixed_delta_time;
        objects[i]->mesh->v *= 0.98f;
        objects[i]->mesh->w *= 0.98f;
        for (int j = 0; j < objects.size(); j++)
        {
            if (i != j && objects[i]->GetTag() != "Wall") {
                objects[i]->CollisionHandler(objects[j], J_mem[i], ri_mem[i]);
            }
        }
    }
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i]->GetTag() != "Wall")
        {
            objects[i]->FixedUpdate(J_mem[i], ri_mem[i]);
        }
    }
    //for (auto& object_1 : objects) {
    //    for (auto& object_2 : objects) {
    //        if (object_1 != object_2 && object_1->GetTag() != "Wall") {
    //            object_1->CollisionHandler(object_2, J_mem, ri_mem);
    //        }
    //    }
    //}


    // 更新位置
    //for (auto& object : objects) {
    //    object->FixedUpdate();
    //}
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

void Scene::Reset()
{
    objects.clear();
}