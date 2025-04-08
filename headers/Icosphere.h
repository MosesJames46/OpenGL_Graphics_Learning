#pragma once
#include "Shapes.h"
#include "Mesh.h"
#include "Shader.h"

class Icosphere : public Shape {
public: 
	float golden_ratio = 1.618033;
	float width = 0.85065f;
	float height = 0.525731f;

	Icosphere(Shader& shader) : shader(shader) {
		generate_icosahedron(vertices, indices);
		gen_bind_format(vertices, indices, VAO, VBO, EBO);
		set_attributes(0, 3, 8, 0);
		set_attributes(1, 3, 8, 3);
		set_attributes(2, 2, 8, 6);
		unbind_buffers_and_attribute_pointer();
	}

	void generate_icosahedron(std::vector<float>& vertices, std::vector<unsigned int>& indices) {
		constexpr float X = 0.525731f;
		constexpr float Z = 0.850651f;

		// Each vertex: x, y, z, r, g, b, u, v
		vertices = {
			-X, 0.0f,  Z,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // 0
			 X, 0.0f,  Z,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // 1
			-X, 0.0f, -Z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // 2
			 X, 0.0f, -Z,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // 3
			 0.0f,  Z,  X,   1.0f, 0.0f, 1.0f,   0.5f, 1.0f,  // 4
			 0.0f,  Z, -X,   0.0f, 1.0f, 1.0f,   0.5f, 0.0f,  // 5
			 0.0f, -Z,  X,   1.0f, 1.0f, 1.0f,   0.5f, 0.5f,  // 6
			 0.0f, -Z, -X,   0.5f, 0.5f, 0.5f,   0.5f, 0.2f,  // 7
			 Z,  X, 0.0f,    0.2f, 0.8f, 0.2f,   1.0f, 0.5f,  // 8
			-Z,  X, 0.0f,    0.8f, 0.2f, 0.2f,   0.0f, 0.5f,  // 9
			 Z, -X, 0.0f,    0.6f, 0.3f, 0.7f,   0.8f, 0.3f,  // 10
			-Z, -X, 0.0f,    0.3f, 0.7f, 0.6f,   0.2f, 0.3f   // 11
		};

		// Same index list as before
		indices = {
			0, 4, 1,    0, 9, 4,    9, 5, 4,
			4, 5, 8,    4, 8, 1,    8, 10, 1,
			8, 3, 10,   5, 3, 8,    5, 2, 3,
			2, 7, 3,    7, 10, 3,   7, 6, 10,
			7, 11, 6,   0, 6, 11,   0, 1, 6,
			6, 1, 10,   9, 0, 11,   9, 11, 2,
			9, 2, 5,    7, 2, 11
		};
	}
	template <typename T>
	void print_value(T value) {
		std::cout << value;
	}

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO, VBO, EBO;
	Shader shader;
};