#include <bunny.h>
#include "load_obj.h"
#include "object.h"
#include "transform.h"
#include <random>

Bunny::Bunny():Mesh(std::vector<MeshVertex>(),
                    std::vector<UVec3>(),
                    GL_STREAM_DRAW, GL_STATIC_DRAW,
                    true)
{
    auto bunny = makeMeshObject("D:/ShanghaiTech/CGI/c/MassRbSim/assets/stanford_bunny.obj", Vec3f(1, 1, 1), 1);
    auto ver = bunny->vertices;
    auto nor = bunny->normals;
    auto vi = bunny->v_indices;
    auto ni = bunny->n_indices;
    std::vector<Vec2> uv = { Vec2(0,0),Vec2(0,1),Vec2(1,1) };
    for (int i = 0; i < vi.size(); i++)
    {
        auto vertex = Vec3(ver[vi[i]].x(), ver[vi[i]].y(), ver[vi[i]].z());
        auto normal = Vec3(nor[ni[i]].x(), ver[ni[i]].y(), ver[ni[i]].z());
        vertices.push_back(MeshVertex(vertex, normal, uv[i % 3]));
        if (i % 3 == 0)
        {
            indices.push_back(UVec3(i, i + 1, i + 2));
        }
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * this->vertices.size(), this->vertices.data(), this->buffer_data_usage_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, uv));
    glEnableVertexAttribArray(2);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(UVec3) * this->indices.size(), this->indices.data(), this->buffer_data_usage_ebo);
    glBindVertexArray(0);
}

void Bunny::FixedUpdate()
{
    object->transform->SetPos(object->transform->GetPos() - Vec3(float(rand() % 10) / 10, 0, 0));
    return;
}