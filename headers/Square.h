#pragma once
#include "Shapes.h"
#include "Shader.h"

class Square : public Shape {
public:
	Square() {
		gen_bind_format(square_vertices, square_indices, square_VAO, square_VBO, square_EBO);
		set_attributes(0, 3, 3, 0);
		unbind_buffers_and_attribute_pointer();
	}

	void draw(Shader shader, int number_of_indices);

private:
	std::vector<float> square_vertices{
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	std::vector<unsigned int> square_indices{
		0, 1, 2,
		0, 2, 3
	};

	unsigned int square_VAO, square_VBO, square_EBO;
};