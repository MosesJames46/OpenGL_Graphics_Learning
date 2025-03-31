#pragma once
#include "Shapes.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

class Triangle : public Shape {
public:
	unsigned int triangle_VAO, triangle_VBO;
	
	Triangle() {
		initialize_mesh();
		ready_buffers();
	}

	Triangle(const char* image_path) {
		initialize_mesh();
		ready_buffers();
	}

	void draw(Shader shader);
	void draw(Shader& shader, std::vector<const char*>&& uniform_names);
	void draw(Shader& shader, std::vector<const char*>& uniform_names);

	void ready_buffers();
	void initialize_mesh();
	

	template <typename T>
	void print_T(T object) {
		std::cout << object;
	}

	Mesh vertex_data;
	std::vector<Texture> triangle_textures;
};
