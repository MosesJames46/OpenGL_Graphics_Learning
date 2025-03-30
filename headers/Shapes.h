#pragma once
#include "libs.h"
#include <iostream>
#include <vector>


class Shape {
public:
	void generate_and_bind_buffers(unsigned int& uninitialized_VAO, unsigned int& uninitialized_VBO, unsigned int& uninitialized_EBO);
	void generate_and_bind_buffers(unsigned int& uninitialized_VAO, unsigned int& uninitialized_VBO);

	void format_buffer(std::vector<float>& vertex_data, GLenum draw_type);
	void format_buffer(std::vector<float>& vertex_data, std::vector<unsigned int>& index_data, GLenum draw_type);

	void set_attributes(int attribute_position, int attribute_size, GLenum numerical_type, GLboolean normalized, int stride_size, int offset);
	void set_attributes(int attribute_position, int attribute_size, int stried_size, int offset);

	void gen_bind_format(std::vector<float>& vertices, std::vector<unsigned int>& indices, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
	void gen_bind_format(std::vector<float>& vertices, unsigned int& VAO, unsigned int& VBO);
	void unbind_buffers_and_attribute_pointer();

	template <typename T>
	void print_data(T data) {
		std::cout << data << "\n";
	}

protected:
	struct Vertex {
		std::vector<float> coordinates;
		std::vector<float> colors;
		Vertex(float coordinates, float colors) : coordinates(coordinates), colors(colors) {};
	};
};

