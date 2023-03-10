#include <ball.h>
#include <mesh.h>
#include <object.h>
#include <transform.h>

Ball::Ball() : Mesh(MeshPrimitiveType::sphere) {

	std::vector<MeshVertex> vertices = this->vertices;
	int vertices_num = vertices.size();

	mass = 0;
	I_ref = Mat3(0.0);
	v = Vec3(0.0);
	w = Vec3(0.0); 
	restitution = 1.0f;


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


}


void Ball::CollisionHandler(std::shared_ptr<Object> obj, std::vector<Vec3> & J_mem, std::vector<Vec3> & ri_mem) {

	//// 初筛
	//if (obj->GetTag() == "Ball") {
	//	if (glm::distance(object->transform->position, obj->transform->position) > 2) {
	//		return;
	//	}
	//}


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

		//// 防止抖动
		//if (abs(vi.y + 9.8f * fixed_delta_time) < 4.0f * fixed_delta_time) {
		//	restitution = 0;
		//}
		//else
		//{
		//	restitution = 1.0f;
		//}
		//

		// R I RT
		Mat3 inv_I = R * glm::inverse(I_ref) * glm::transpose(R);

		// 叉乘向量转化为矩阵
		Mat3 Rstar = Mat3({ 0,-ri[2],ri[1] }, { ri[2],0,-ri[0] }, { -ri[1],ri[0], 0 });
		Mat3 K = glm::transpose(Rstar) * inv_I * Rstar;

		K[0][0] += 1.0 / mass;
		K[1][1] += 1.0 / mass;
		K[2][2] += 1.0 / mass;

		obj->mesh->GetNP(x + ri, P, N);

		Vec3 vi_n = glm::dot(vi, N) * N;
		Vec3 vi_t = vi - vi_n;

		float u_n = 0.3f;
		float u_t = 1 - u_n * (1 + u_n) * glm::length(vi_n) / glm::length(vi_t);
		if (u_t < 0) {
			u_t = 0;
		}

		Vec3 vi_new = -u_n * vi_n + u_t * vi_t;


		Vec3 temp = -vi + vi_new * restitution;

		// restitution正常是0.5 轻微抖动时变为0
		//Vec3 temp = -vi - restitution * (vi.y * N);
		Vec3 J = glm::inverse(K) * temp;

		// 更新速度和角速度
		//v += J / mass;
		//w += inv_I * glm::cross(ri, J);

		//储存J，ri
		J_mem.push_back(J / mass);
		ri_mem.push_back(inv_I * glm::cross(ri, J));
	}

}

void Ball::FixedUpdate(std::vector<Vec3>& J_mem, std::vector<Vec3>& ri_mem)
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
	object->transform->SetRotation(glm::normalize(q));
	return;
}

void Ball::GetNP(Vec3 vertex, Vec3& P, Vec3& N)
{
	Vec3 x = object->transform->position;
	N = glm::normalize(vertex - x);
	float len = vertices[0].position.x / glm::normalize(vertices[0].position).x;
	P = len * N + x;
	return;
}

