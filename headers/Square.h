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
	void bind_textures(Shader& shader, std::vector<const char*>&& uniform_names);
	void bind_textures(Shader& shader, std::vector<const char*>& uniform_names);
	void add_textures(std::vector<const char*> file_paths);

	void draw(Shader shader, int number_of_indices);
	void draw(Shader& shader, int number_of_indices, std::vector<const char*>&& uniform_names);
	void draw(Shader& shader, int number_of_indices, std::vector<const char*>& uniform_names);

private:
	Mesh mesh;

	std::vector<unsigned int> square_indices{
		0, 1, 2,
		0, 2, 3
	};

	unsigned int square_VAO, square_VBO, square_EBO;
	std::vector<Texture> square_textures;
};