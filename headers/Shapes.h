#pragma once
#include "libs.h"
#include <iostream>
#include <vector>
#include "Texture.h"

class Mesh;

class Shape {
public:
	virtual void generate_normals(std::vector<float>& vertices, std::vector<unsigned>& indices, std::vector<float>& normals);
	virtual void generate_basic_normals(std::vector<float>& vertices, std::vector<float>& normals);
	virtual void generate_indices(std::vector<unsigned int>& indices) {};
	virtual void generate_vertices(std::vector<float>& vertices) {};

	virtual void generate_mesh(Mesh& mesh) {};

	virtual void set_scale(float scalar, glm::vec3& scale);

	static glm::vec3 calculate_normals(glm::vec3& a, glm::vec3& b, glm::vec3& c);

	template <typename T>
	void print_data(T data) {
		std::cout << data << "\n";
	}
};

