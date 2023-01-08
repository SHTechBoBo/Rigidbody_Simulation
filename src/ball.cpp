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
	restitution = 0.5f;

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

//void Ball::CollisionDetect(std::shared_ptr<Mesh> mesh) {

//	std::shared_ptr<Object> obj = mesh->GetObject();

	//if (obj->GetTag() == "Wall") {
		// position rotation scale
		//Vec3 wall_P = obj->transform->position;
		//Mat3 wall_R = glm::mat3_cast(obj->transform->rotation) * Mat3(1.0);
		//Vec3 wall_S = obj->transform->scale;
	
		//// six surface
		//Vec3 left = wall_R * (wall_P - Vec3(1, 0, 0) * wall_S.x);
		//Vec3 right = wall_R * (wall_P + Vec3(1, 0, 0) * wall_S.x);
		//Vec3 up = wall_R * (wall_P + Vec3(0, 1, 0) * wall_S.y);
		//Vec3 down = wall_R * (wall_P - Vec3(0, 1, 0) * wall_S.y);
		//Vec3 front = wall_R * (wall_P - Vec3(0, 0, 1) * wall_S.z);
		//Vec3 behind = wall_R * (wall_P + Vec3(0, 0, 1) * wall_S.z);
		//
		//std::vector<Vec3> surfaces{ left, right, up, down, front, behind };
		//
		//// six normal
		//Vec3 left_normal = glm::normalize(wall_R * Vec3(-1, 0, 0));
		//Vec3 right_normal = glm::normalize(wall_R * Vec3(1, 0, 0));
		//Vec3 up_normal = glm::normalize(wall_R * Vec3(0, 1, 0));
		//Vec3 down_normal = glm::normalize(wall_R * Vec3(0, -1, 0));
		//Vec3 front_normal = glm::normalize(wall_R * Vec3(0, 0, -1));
		//Vec3 behind_normal = glm::normalize(wall_R * Vec3(0, 0, 1));

		//std::vector<Vec3> normals{ left_normal, right_normal, up_normal, down_normal, front_normal, behind_normal };

		//Mat3 R = glm::mat3_cast(object->transform->rotation) * Mat3(1.0);
		//Vec3 sum_ri = Vec3(0.0);
		//float sum = 0;

		//std::vector<MeshVertex> vertices = this->vertices;
		//int vertices_num = vertices.size();

		//Vec3 x = object->transform->position;

		//int surface_id = -1;

		// 计算各个点
		//for (int i = 0; i < vertices_num; i++) {
		//	Vec3 ri = R * vertices[i].position;
		//	Vec3 xi = x + ri;
		//
		//	int count = 0;
		//	float dis = -10000;
		//	int close_surface_id = -1;
		//	for (int j = 0; j < 6; j++) {
		//		float tmp = glm::dot(xi - obj->mesh->vertices[6 * j].position, obj->mesh->vertices[6 * j].normal);
		//		// 在一个面内
		//		if (tmp < 0) {
		//			count++;
		//			// 算到那个面最近
		//			if (tmp > dis) {
		//				dis = tmp;
		//				close_surface_id = j;
		//			}
		//		}
				// 在所有面内才算真的在内部
				//if (count == 6) {
				//	surface_id = close_surface_id;
//					CollisionHandler(obj);
				//	break;
				//}
		//	}
		//}

		//if (surface_id != -1) {
		//	CollisionHandler(surfaces[surface_id] + normals[surface_id]*1e-2f, normals[surface_id]);
		//}

	//}
//}


void Ball::CollisionHandler(std::shared_ptr<Object> obj, std::vector<Vec3> & J_mem, std::vector<Vec3> & ri_mem) {
	// 重力加速度
	//v[1] -= 9.8f * fixed_delta_time;
	//
	//v *= 0.98f;
	//w *= 0.98f;

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
		Vec3 temp = -vi - restitution * (vi.y * N);
		Vec3 J = glm::inverse(K) * temp;

		// 更新速度和角速度
		//v += J / mass;
		//w += inv_I * glm::cross(ri, J);

		//储存J，ri
		J_mem.push_back(J);
		ri_mem.push_back(ri);
	}

}

void Ball::FixedUpdate(std::vector<Vec3>& J_mem, std::vector<Vec3>& ri_mem)
{
	v[1] -= 9.8f * fixed_delta_time;

	v *= 0.98f;
	w *= 0.98f;
	Mat3 R = glm::mat3_cast(object->transform->rotation) * Mat3(1.0);
	Mat3 inv_I = R * glm::inverse(I_ref) * glm::transpose(R);

	for (int i = 0; i < J_mem.size(); i++)
	{
		v += J_mem[i] / mass;
		w += inv_I * glm::cross(ri_mem[i], J_mem[i]);
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
	object->transform->SetRotation(w);
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