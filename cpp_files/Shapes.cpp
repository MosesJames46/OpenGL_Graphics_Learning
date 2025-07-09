#include "../headers/Shapes.h"

void Shape::generate_normals(std::vector<float>& vertices, std::vector<unsigned int>& indices, std::vector<float>& normals) {
	normals.resize(vertices.size(), 0.0f);
	std::cout << vertices.size() << " " << normals.size() << " " << indices.size() << std::endl;
	for (size_t i = 0; i < indices.size(); i += 3) {
		//std::cout << i << std::endl;
		// Index starts at the beginning of the vertex when multiplied by 3. 
		unsigned int i1 = indices[i] * 3;
		unsigned int i2 = indices[i + 1] * 3;
		unsigned int i3 = indices[i + 2] * 3;

		glm::vec3 v0(vertices[i1], vertices[i1 + 1], vertices[i1 + 2]);
		glm::vec3 v1(vertices[i2], vertices[i2 + 1], vertices[i2 + 2]);
		glm::vec3 v2(vertices[i3], vertices[i3 + 1], vertices[i3 + 2]);

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
		normals[i1] += normal.x;
		normals[i1 + 1] += normal.y;
		normals[i1 + 2] += normal.z;

		normals[i2] += normal.x;
		normals[i2 + 1] += normal.y;
		normals[i2 + 2] += normal.z;

		normals[i3] += normal.x;
		normals[i3 + 1] += normal.y;
		normals[i3 + 2] += normal.z;
	}

	// Normalize normals (to ensure they're unit vectors)
	for (size_t i = 0; i < normals.size(); i += 3) {
		glm::vec3 normal(normals[i], normals[i + 1], normals[i + 2]);
		normal = glm::normalize(normal);
		normals[i] = normal.x;
		normals[i + 1] = normal.y;
		normals[i + 2] = normal.z;
	}
}

void Shape::generate_basic_normals(std::vector<float>& vertices, std::vector<float>& normals) {
	if (!normals.empty()) normals.clear();
	for (int i = 0; i < vertices.size(); i += 3) {
		glm::vec3 n = glm::vec3{ vertices[i], vertices[i + 1], vertices[i + 2] };
		n = glm::normalize(n);
		normals.push_back(n.x);
		normals.push_back(n.y);
		normals.push_back(n.z);
	}
}

void Shape::set_scale(float scalar, glm::vec3& scale) {
	scale *= scalar;
}

glm::vec3 Shape::calculate_normals(glm::vec3& a, glm::vec3& b, glm::vec3& c) {
	glm::vec3 v = b - a;
	glm::vec3 u = c - a;
	return glm::cross(v, u);
}