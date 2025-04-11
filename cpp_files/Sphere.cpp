#include "../headers/Sphere.h"

float pi = 3.14159;

void Sphere::initialize_mesh(float sectors, float stacks) {
	generate_sphere(stacks, sectors);
	generate_indices(stacks, sectors);
	generate_basic_normals();
	sphere_mesh.load_vertices_and_normals(vertices, normals);
	generate_indices(stacks, sectors);
}

void Sphere::set_radius() {
	if (prev_radius == radius) return;
	for (int i = 0; i < sphere_mesh.mesh.size(); i+= 9) {
		sphere_mesh.mesh[i] = (sphere_mesh.mesh[i] / prev_radius) * radius;
		sphere_mesh.mesh[i + 1] = (sphere_mesh.mesh[i + 1] / prev_radius) * radius;
		sphere_mesh.mesh[i + 2] = (sphere_mesh.mesh[i + 2] / prev_radius) * radius;
	}
	prev_radius = radius;
	redraw(shader, sphere_VBO, sphere_mesh);
	unbind_buffers_and_attribute_pointer();
}

void Sphere::ready_buffers() {
	gen_bind_format(sphere_mesh, sphere_indices, sphere_VAO, sphere_VBO, sphere_EBO);
	set_attributes(0, 3, 9, 0);
	set_attributes(1, 3, 9, 3);
	set_attributes(2, 3, 9, 6);
	unbind_buffers_and_attribute_pointer();
}

void Sphere::generate_sphere(int sectors, int stacks) {
	int sector_count = sectors;
	int stack_count = stacks;
	float sector_step = 2 * pi / sector_count; //The percentage needed to complete a full 2 * pi in relation to the sector count total.
	float stack_step = pi / stack_count;// The percentage needed to complete a full pi in relation to the stack count total.
	float xy, x, y, z;
	float half_pi = (pi / 2);
	
	/*
	float current_stack_angle = half_pi - i * stack_step;

	Obtains a percentage of half a circle.
	when i is 0 only have influence in the y.
	When i is half of our sector count, we have both x and y distances equal to unit 1.
	Only when i is equal to the stack count do we enter -half_pi which gives us influence
	in the -y direction. This effectively acts as a force from pi/2 to -pi / 2;
	*/

	for (int i = 0; i <= stack_count; i++) {
		float current_stack_angle = half_pi - i * stack_step;
		xy = radius * cosf(current_stack_angle);
		z = radius * sinf(current_stack_angle);
		for (int j = 0; j <= sector_count; j++) {
			float current_sector_angle = j * sector_step;
			x = xy * cosf(current_sector_angle);
			y = xy * sinf(current_sector_angle);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}
}

void Sphere::generate_indices(int stack, int sector) {
	int k1, k2;
	for (int i = 0; i < stack; i++) {
		k1 = i * (sector + 1);
		k2 = k1 + sector + 1;
		for (int j = 0; j < sector; j++, k1++, k2++) {
			if (i != 0) {
				sphere_indices.push_back(k1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k1 + 1);
			}

			if (i != (stack)) {
				sphere_indices.push_back(k1 + 1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k2 + 1);
			}

			if (i != stack - 1) {
				sphere_indices.push_back(k1 + 1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k2 + 1);
			}

			sphere_indices.push_back(k1);
			sphere_indices.push_back(k2);

			if (i != 0) {
				sphere_indices.push_back(k1);
			}
		}
	}
}

void Sphere::generate_normals() {
	normals.resize(vertices.size(), 0.0f);
	for (size_t i = 0; i < sphere_indices.size(); i += 3) {
		// Get indices of the 3 vertices of the triangle
		unsigned int i1 = sphere_indices[i];
		unsigned int i2 = sphere_indices[i + 1];
		unsigned int i3 = sphere_indices[i + 2];

		glm::vec3 v0(vertices[i1 * 3], vertices[i1 * 3 + 1], vertices[i1 * 3 + 2]);
		glm::vec3 v1(vertices[i2 * 3], vertices[i2 * 3 + 1], vertices[i2 * 3 + 2]);
		glm::vec3 v2(vertices[i3 * 3], vertices[i3 * 3 + 1], vertices[i3 * 3 + 2]);

		// Calculate the normal of the triangle (cross product of two edges)
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;
		glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
		if (std::isnan(normal.x)) {
			normal.x = 0.0f;
			normal.y = 0.0f;
			normal.z = 1.0f;
		}

		// Assign the normal to all three vertices of the triangle
		normals[i1 * 3] += normal.x;
		normals[i1 * 3 + 1] += normal.y;
		normals[i1 * 3 + 2] += normal.z;

		normals[i2 * 3] += normal.x;
		normals[i2 * 3 + 1] += normal.y;
		normals[i2 * 3 + 2] += normal.z;

		normals[i3 * 3] += normal.x;
		normals[i3 * 3 + 1] += normal.y;
		normals[i3 * 3 + 2] += normal.z;
	}

	// Normalize normals (to ensure they're unit vectors)
	for (size_t i = 0; i < normals.size(); i += 3) {
		glm::vec3 normal(normals[i], normals[i + 1], normals[i + 2]);
		normal = glm::normalize(normal);
		normals[i] = normal.x;
		normals[i + 1] = normal.y;
		normals[i + 2] = normal.z;
	}
	//for (int i = 0; i < normals.size(); i += 3) {
	//	std::cout << normals[i] << " " << normals[i + 1] << " " << normals[i + 2] << "\n";
	//}
}
void Sphere::generate_basic_normals() {
	if (!normals.empty()) normals.clear();
	for (int i = 0; i < vertices.size(); i += 3) {
		glm::vec3 n = glm::vec3{ vertices[i], vertices[i + 1], vertices[i + 2] };
		n = glm::normalize(n);
		normals.push_back(n.x);
		normals.push_back(n.y);
		normals.push_back(n.z);
	}
}

void Sphere::set_object_size() {
	ImGui::Text("Radius: ");
	ImGui::PushItemWidth(60);
	ImGui::SameLine();
	ImGui::DragFloat("##float", &radius, 0.01f, 0.1f, (std::numeric_limits<float>::max)());
	ImGui::PopItemWidth();

	set_radius();
}

void Sphere::sphere_options(const char* uniform_color_name, const char* uniform_position_name) {
	ImGui::Begin(sphere_name.c_str());
	set_color(shader, uniform_color_name, &color[0]);
	set_object_size();
	ImGui::End();
}

void Sphere::draw(const char* uniform_color_name, const char* uniform_position_name) {
	Shape::draw(shader, sphere_VAO, sphere_indices.size(), uniform_color_name, &color[0], 
		uniform_position_name, &position[0], sphere_name.c_str(), func);
}

