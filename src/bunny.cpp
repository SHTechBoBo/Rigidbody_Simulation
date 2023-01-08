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
    auto bunny = makeMeshObject(GetFilePath("assets/ubunny.obj").c_str(), Vec3f(0, 0, 0), 1);
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

	mass = 0;
	I_ref = Mat3(0.0);
	v = Vec3{ 10,0,0 };
	w = Vec3{40,0,0};
	restitution = 1.0f;

	std::vector<MeshVertex> vertices = this->vertices;
	int vertices_num = vertices.size();

	for (int i = 0; i < vertices_num; i++) {
		// 一个三角形认为质量是1
		mass += 1;

		float x = vertices[i].position[0];
		float y = vertices[i].position[1];
		float z = vertices[i].position[2];

		// 转动惯量矩阵每一项都要乘以质量 但因为是1所以省略  
		float diag = sqrt(x * x + y * y + z * z);

		I_ref[0][0] += diag;
		I_ref[1][1] += diag;
		I_ref[2][2] += diag;

		I_ref[0][0] -= x * x;
		I_ref[0][1] -= x * y;
		I_ref[0][2] -= x * z;

		I_ref[1][0] -= y * x;
		I_ref[1][1] -= y * y;
		I_ref[1][2] -= y * z;

		I_ref[2][0] -= z * x;
		I_ref[2][1] -= z * y;
		I_ref[2][2] -= z * z;
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

void Bunny::CollisionHandler(std::shared_ptr<Object> obj, std::vector<Vec3>& J_mem, std::vector<Vec3>& ri_mem) {


	// 旋转矩阵
	Mat3 R = glm::mat3_cast(object->transform->rotation) * Mat3(1.0);
	Vec3 sum_ri = Vec3(0.0);
	float sum = 0;

	Vec3 x = object->transform->position;
	Vec3 P, N;

	std::vector<MeshVertex> vertices = this->vertices;
	int vertices_num = vertices.size();

	// 计算各个点的力矩
	for (int i = 0; i < vertices_num; i++) {
		Vec3 ri = R * vertices[i].position;
		Vec3 xi = x + ri;
		Vec3 vi = v + glm::cross(w, ri);

		// 有没有陷入其他物体
		obj->mesh->GetNP(xi, P, N);
		if (glm::dot(xi - P, N) < 0 && glm::dot(vi, N) < 0) {
			sum_ri += ri;
			sum++;
		}
	}

	if (sum != 0) {
		Vec3 ri = sum_ri / sum;
		Vec3 vi = v + glm::cross(w, ri);

		// 防止抖动
		if (abs(vi.y + 9.8f * fixed_delta_time) < 4.0f * fixed_delta_time) {
			restitution = 0;
		}
		else
		{
			restitution = 1.0f;
		}

		// R I RT
		Mat3 inv_I = R * glm::inverse(I_ref) * glm::transpose(R);

		// 叉乘向量转化为矩阵
		Mat3 Rstar = Mat3({ 0,-ri[2],ri[1] }, { ri[2],0,-ri[0] }, { -ri[1],ri[0], 0 });
		Mat3 K = glm::transpose(Rstar) * inv_I * Rstar;

		K[0][0] += 1.0 / mass;
		K[1][1] += 1.0 / mass;
		K[2][2] += 1.0 / mass;

		obj->mesh->GetNP(x + ri, P, N);

		// restitution正常是0.5 轻微抖动时变为0

		Vec3 vi_n = glm::dot(vi, N) * N;
		Vec3 vi_t = vi - vi_n;
		float u_n = 0.8f;
		float u_t = 1 - u_n * (1 + u_n) * glm::length(vi_n) / glm::length(vi_t);
		if (u_t < 0) {
			u_t = 0;
		}

		Vec3 vi_new = -u_n * vi_n + u_t * vi_t;


		Vec3 temp = -vi +  restitution * vi_new;
		// Vec3 temp = -vi - restitution * (vi.y * N);

		Vec3 J = glm::inverse(K) * temp;

		// 更新速度和角速度
		//v += J / mass;
		//w += inv_I * glm::cross(ri, J);

		//储存J，ri
		J_mem.push_back(J / mass);

		//Vec3 delta_w = inv_I * glm::cross(ri, J);
		//if (glm::length(delta_w) < 4 * fixed_delta_time) {
		//	delta_w = Vec3(0.0f);
		//}
		//ri_mem.push_back(delta_w);

		ri_mem.push_back(inv_I * glm::cross(ri, J));
	}

}


void Bunny::FixedUpdate(std::vector<Vec3>& J_mem, std::vector<Vec3>& ri_mem)
{
	Mat3 R = glm::mat3_cast(object->transform->rotation) * Mat3(1.0);
	Mat3 inv_I = R * glm::inverse(I_ref) * glm::transpose(R);

	for (int i = 0; i < J_mem.size(); i++)
	{
		v += J_mem[i];
		w += ri_mem[i];
	}

	Vec3 x = object->transform->position;
	Quat q = object->transform->rotation;

	// CollisionHandler(Vec3(0, 0.05f, 0), Vec3(0, 1, 0));

	x += fixed_delta_time * v;
	Quat wq = Quat(w.x, w.y, w.z, 0);
	Quat temp_q = wq * q;

	q.x += 0.5f * fixed_delta_time * temp_q.x;
	q.y += 0.5f * fixed_delta_time * temp_q.y;
	q.z += 0.5f * fixed_delta_time * temp_q.z;
	q.w += 0.5f * fixed_delta_time * temp_q.w;

	// std::cout << x.x << " " << x.y << " " << x.z << "\n";
	object->transform->SetPos(x);

	/*std::cout << "w:" << w.x << " " << w.y << " " << w.z << "\n";
	std::cout << "q:" << q.x << " " << q.y << " " << q.z << " " << q.w << "\n";
	std::cout << "n_q:" << glm::normalize(q).x << " " << glm::normalize(q).y << " " << glm::normalize(q).z << " " << glm::normalize(q).w << "\n";*/
	object->transform->SetRotation(glm::normalize(q));

	return;
}