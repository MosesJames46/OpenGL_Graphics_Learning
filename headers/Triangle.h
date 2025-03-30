#pragma once
#include "Shapes.h"
#include "Shader.h"

class Triangle : public Shape {
public:
	unsigned int triangle_VAO, triangle_VBO;
	

	std::vector<float> triangle_vertices = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	Triangle() {
		gen_bind_format(triangle_vertices, triangle_VAO, triangle_VBO);
		set_attributes(0, 3, 6, 0);
		set_attributes(1, 3, 6, 3);
		unbind_buffers_and_attribute_pointer();
	}

	void draw(Shader shader, int start, int vertex_count);
	
	template <typename T>
	void print_T(T object) {
		std::cout << object;
	}

private:
	std::vector<Vertex> vertex_data;
};
