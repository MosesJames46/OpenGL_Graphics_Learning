#include "../headers/Mesh.h"

void Mesh::load_vertex(Vertex vertex) {
	mesh.push_back(vertex.coordinates.x);
	mesh.push_back(vertex.coordinates.y);
	mesh.push_back(vertex.coordinates.z);
	mesh.push_back(vertex.color.x);
	mesh.push_back(vertex.color.y);
	mesh.push_back(vertex.color.z);
	mesh.push_back(vertex.texture_coordinates.x);
	mesh.push_back(vertex.texture_coordinates.y);
}

void Mesh::load_vertices() {
	mesh.resize((vertices.size() / 3) * 9);
	int size = mesh.size();
	int vertices_index = 0;
	for (int i = 0; i < size; i+= 9) {
		mesh[i] = vertices[vertices_index];
		mesh[i + 1] = vertices[vertices_index + 1];
		mesh[i + 2] = vertices[vertices_index + 2];
		vertices_index += 3;
	}
}

void Mesh::load_vertices_and_colors() {
	mesh.resize(vertices.size() * 3);
	int size = mesh.size();
	for (int i = 0; i < size; i += 9) {
		mesh[i] = vertices[i];
		mesh[i + 1] = vertices[i + 1];
		mesh[i + 2] = vertices[i + 2];
		
		mesh[i + 6] = vertices[i + 6];
		mesh[i + 7] = vertices[i + 7];
		mesh[i + 8] = vertices[i + 8];
		//std::cout << mesh[i + 6] << " " << mesh[i + 7] << " " << mesh[i + 8];
	}
}

void Mesh::load_normals() {
	int size = mesh.size();
	int normals_index = 0;
	for (int i = 6; i < size; i += 9) {
		mesh[i] = normals[normals_index];
		mesh[i + 1] = normals[normals_index + 1];
		mesh[i + 2] = normals[normals_index + 2];

		normals_index += 3;
	}
}

void Mesh::load_vertices_and_normals() {
	load_vertices();
	load_normals();
}

void Mesh::generate_sphere(int sectors, int stacks) {
	float pi = 3.1415926;
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
		xy = uniform_scale * cosf(current_stack_angle);
		z = uniform_scale * sinf(current_stack_angle);
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

void Mesh::generate_indices(int stack, int sector) {
	int k1, k2;
	for (int i = 0; i < stack; i++) {
		k1 = i * (sector + 1);
		k2 = k1 + sector + 1;
		for (int j = 0; j < sector; j++, k1++, k2++) {
			if (i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stack)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}

			if (i != stack - 1) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}

			indices.push_back(k1);
			indices.push_back(k2);

			if (i != 0) {
				indices.push_back(k1);
			}
		}
	}
}

void Mesh::generate_normals() {
	normals.resize(vertices.size(), 0.0f);
	for (size_t i = 0; i < indices.size(); i += 3) {
		// Get indices of the 3 vertices of the triangle
		unsigned int i1 = indices[i];
		unsigned int i2 = indices[i + 1];
		unsigned int i3 = indices[i + 2];

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
void Mesh::generate_basic_normals() {
	if (!normals.empty()) normals.clear();
	for (int i = 0; i < vertices.size(); i += 3) {
		glm::vec3 n = glm::vec3{ vertices[i], vertices[i + 1], vertices[i + 2] };
		n = glm::normalize(n);
		normals.push_back(n.x);
		normals.push_back(n.y);
		normals.push_back(n.z);
	}
}