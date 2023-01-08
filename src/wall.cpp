#include <wall.h>
#include "load_obj.h"
#include "object.h"
#include "transform.h"
#include <random>

Wall::Wall(Vec3 scale, Vec3 rotate) : Mesh(std::vector<MeshVertex>(),
    std::vector<UVec3>(),
    GL_STREAM_DRAW, GL_STATIC_DRAW,
    true)
{
    //Mat4 t = { scale.x, 0, 0, 0,
    //        0, scale.y, 0, 0,
    //        0, 0, scale.z, 0,
    //        0, 0, 0, 1 };
    Mat4 r = Mat4(1.0f);
    Mat4 m = Mat4(1.0f);
    Mat4 t = glm::scale(r, scale);
    m = glm::rotate(r, rotate.x, Vec3(1, 0, 0)) * glm::rotate(r, rotate.y, Vec3(0, 1, 0)) * glm::rotate(r, rotate.z, Vec3(0, 0, 1)) * t;

    vertices = {
        //MeshVertex(Vec3(-0.5, -0.5, 0), Vec3(0.0,  0.0, -1.0), Vec2(0.0, 0.0)), // forward
        //MeshVertex(Vec3(0.5, -0.5, 0), Vec3(0.0,  0.0, -1.0), Vec2(1.0, 0.0)),
        //MeshVertex(Vec3(0.5,  0.5, 0), Vec3(0.0,  0.0, -1.0), Vec2(1.0, 1.0)),
        //MeshVertex(Vec3(0.5,  0.5, 0), Vec3(0.0,  0.0, -1.0), Vec2(1.0, 1.0)),
        //MeshVertex(Vec3(-0.5,  0.5, 0), Vec3(0.0,  0.0, -1.0), Vec2(0.0, 1.0)),
        //MeshVertex(Vec3(-0.5, -0.5, 0), Vec3(0.0,  0.0, -1.0), Vec2(0.0, 0.0))
        MeshVertex(Vec3(-0.5, -0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(0.0, 0.0)), // back
        MeshVertex(Vec3(0.5, -0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(1.0, 0.0)),
        MeshVertex(Vec3(0.5,  0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(1.0, 1.0)),
        MeshVertex(Vec3(0.5,  0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(1.0, 1.0)),
        MeshVertex(Vec3(-0.5,  0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(0.0, 1.0)),
        MeshVertex(Vec3(-0.5, -0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(0.0, 0.0)),

        MeshVertex(Vec3(-0.5, -0.5, -0.5), Vec3(0.0,  0.0, -1.0), Vec2(0.0, 0.0)), // forward
        MeshVertex(Vec3(0.5, -0.5, -0.5), Vec3(0.0,  0.0, -1.0), Vec2(1.0, 0.0)),
        MeshVertex(Vec3(0.5,  0.5, -0.5), Vec3(0.0,  0.0, -1.0), Vec2(1.0, 1.0)),
        MeshVertex(Vec3(0.5,  0.5, -0.5), Vec3(0.0,  0.0, -1.0), Vec2(1.0, 1.0)),
        MeshVertex(Vec3(-0.5,  0.5, -0.5), Vec3(0.0,  0.0, -1.0), Vec2(0.0, 1.0)),
        MeshVertex(Vec3(-0.5, -0.5, -0.5), Vec3(0.0,  0.0, -1.0), Vec2(0.0, 0.0)),

        //MeshVertex(Vec3(-0.5, -0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(0.0, 0.0)), // back
        //MeshVertex(Vec3(0.5, -0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(1.0, 0.0)),
        //MeshVertex(Vec3(0.5,  0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(1.0, 1.0)),
        //MeshVertex(Vec3(0.5,  0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(1.0, 1.0)),
        //MeshVertex(Vec3(-0.5,  0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(0.0, 1.0)),
        //MeshVertex(Vec3(-0.5, -0.5,  0.5), Vec3(0.0,  0.0,  1.0), Vec2(0.0, 0.0)),
        
        MeshVertex(Vec3(-0.5,  0.5,  0.5), Vec3(-1.0,  0.0,  0.0), Vec2(1.0, 0.0)), // left
        MeshVertex(Vec3(-0.5,  0.5, -0.5), Vec3(-1.0,  0.0,  0.0), Vec2(1.0, 1.0)),
        MeshVertex(Vec3(-0.5, -0.5, -0.5), Vec3(-1.0,  0.0,  0.0), Vec2(0.0, 1.0)),
        MeshVertex(Vec3(-0.5, -0.5, -0.5), Vec3(-1.0,  0.0,  0.0), Vec2(0.0, 1.0)),
        MeshVertex(Vec3(-0.5, -0.5,  0.5), Vec3(-1.0,  0.0,  0.0), Vec2(0.0, 0.0)),
        MeshVertex(Vec3(-0.5,  0.5,  0.5), Vec3(-1.0,  0.0,  0.0), Vec2(1.0, 0.0)),
        
        MeshVertex(Vec3(0.5,  0.5,  0.5), Vec3(1.0,  0.0,  0.0), Vec2(1.0, 0.0)), // right
        MeshVertex(Vec3(0.5,  0.5, -0.5), Vec3(1.0,  0.0,  0.0), Vec2(1.0, 1.0)),
        MeshVertex(Vec3(0.5, -0.5, -0.5), Vec3(1.0,  0.0,  0.0), Vec2(0.0, 1.0)),
        MeshVertex(Vec3(0.5, -0.5, -0.5), Vec3(1.0,  0.0,  0.0), Vec2(0.0, 1.0)),
        MeshVertex(Vec3(0.5, -0.5,  0.5), Vec3(1.0,  0.0,  0.0), Vec2(0.0, 0.0)),
        MeshVertex(Vec3(0.5,  0.5,  0.5), Vec3(1.0,  0.0,  0.0), Vec2(1.0, 0.0)),
        
        MeshVertex(Vec3(-0.5, -0.5, -0.5), Vec3(0.0, -1.0,  0.0), Vec2(0.0, 1.0)), // down
        MeshVertex(Vec3(0.5, -0.5, -0.5), Vec3(0.0, -1.0,  0.0), Vec2(1.0, 1.0)),
        MeshVertex(Vec3(0.5, -0.5,  0.5), Vec3(0.0, -1.0,  0.0), Vec2(1.0, 0.0)),
        MeshVertex(Vec3(0.5, -0.5,  0.5), Vec3(0.0, -1.0,  0.0), Vec2(1.0, 0.0)),
        MeshVertex(Vec3(-0.5, -0.5,  0.5), Vec3(0.0, -1.0,  0.0), Vec2(0.0, 0.0)),
        MeshVertex(Vec3(-0.5, -0.5, -0.5), Vec3(0.0, -1.0,  0.0), Vec2(0.0, 1.0)),
        
        MeshVertex(Vec3(-0.5,  0.5, -0.5), Vec3(0.0,  1.0,  0.0), Vec2(0.0, 1.0)), // up
        MeshVertex(Vec3(0.5,  0.5, -0.5), Vec3(0.0,  1.0,  0.0), Vec2(1.0, 1.0)),
        MeshVertex(Vec3(0.5,  0.5,  0.5), Vec3(0.0,  1.0,  0.0), Vec2(1.0, 0.0)),
        MeshVertex(Vec3(0.5,  0.5,  0.5), Vec3(0.0,  1.0,  0.0), Vec2(1.0, 0.0)),
        MeshVertex(Vec3(-0.5,  0.5,  0.5), Vec3(0.0,  1.0,  0.0), Vec2(0.0, 0.0)),
        MeshVertex(Vec3(-0.5,  0.5, -0.5), Vec3(0.0,  1.0,  0.0), Vec2(0.0, 1.0)),
    };
    indices = {
        UVec3(0,  1,  2), UVec3(3,  4,  5),
        UVec3(6,  7,  8), UVec3(9, 10, 11),
        UVec3(12, 13, 14), UVec3(15, 16, 17),
        UVec3(18, 19, 20), UVec3(21, 22, 23),
        UVec3(24, 25, 26), UVec3(27, 28, 29),
        UVec3(30, 31, 32), UVec3(33, 34, 35),
    };

    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i].position = Vec3(m * Vec4(vertices[i].position, 1.0));
    }
    float sign;
    for (int i = 0; i < 6; i++)
    {
        auto normal = glm::normalize(glm::cross(vertices[i * 6 + 2].position - vertices[i * 6].position,vertices[i * 6 + 1].position - vertices[i * 6].position));
        if (i % 2 == 0)
            sign = -1;
        else
            sign = 1;
        for (int j = 0; j < 6; j++)
        {
            vertices[i * 6 + j].normal = sign * normal;
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

void Wall::GetNP(Vec3 vertex, Vec3& P, Vec3& N)
{
    int count = 0;
    float dis = -10000;
    int close_surface_id = -1;
    for (int j = 0; j < 6; j++) {
        float tmp = glm::dot(vertex - vertices[6 * j].position, vertices[6 * j].normal);
        // 算到那个面最近
        if (tmp > dis) {
            dis = tmp;
            close_surface_id = j;
            if (tmp > 0)
            {
                P = vertices[6 * j].position;
                N = vertices[6 * j].normal;
                return;
            }
            if(tmp < 0)
            {
                count++;
            }
        }
    }
    P = vertices[6 * close_surface_id].position;
    N = vertices[6 * close_surface_id].normal;
    return;
}
