#pragma once
#include "Shapes.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

class Square : public Shape {
public:
	Square() {
		initialize_mesh();
		ready_buffers();
	}

	Square(const char* image_path) {
		initialize_mesh();
		ready_buffers();
	}

	void initialize_mesh();
	void ready_buffers();


	Mesh mesh;

	std::vector<unsigned int> square_indices{
		0, 1, 2,
		0, 2, 3
	};

	unsigned int square_VAO, square_VBO, square_EBO;
	std::vector<Texture> square_textures;
};