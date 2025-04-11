#pragma once
#include "libs.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include <functional>
#include <iostream>
#include <vector>

static enum fragment_shader_type {
	STANDARD_SHADER, LIGHT_SHADER
};

class Shape {
public:
	void generate_and_bind_buffers(unsigned int& uninitialized_VAO, unsigned int& uninitialized_VBO, unsigned int& uninitialized_EBO);
	void generate_and_bind_buffers(unsigned int& uninitialized_VAO, unsigned int& uninitialized_VBO);

	void format_buffer(std::vector<float>& vertex_data, GLenum draw_type);
	void format_buffer(Mesh& vertex_data, GLenum draw_type);
	void format_buffer(std::vector<float>& vertex_data, std::vector<unsigned int>& index_data, GLenum draw_type);
	void format_buffer(Mesh& vertex_data, std::vector<unsigned int>& index_data, GLenum draw_type);

	void set_attributes(int attribute_position, int attribute_size, GLenum numerical_type, GLboolean normalized, int stride_size, int offset);
	void set_attributes(int attribute_position, int attribute_size, int stried_size, int offset);

	void gen_bind_format(std::vector<float>& vertices, std::vector<unsigned int>& indices, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
	void gen_bind_format(std::vector<float>& vertices, unsigned int& VAO, unsigned int& VBO);
	void gen_bind_format(Mesh& vertices, std::vector<unsigned int>& indices, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
	void gen_bind_format(Mesh& vertices, unsigned int& VAO, unsigned int& VBO);

	void unbind_buffers_and_attribute_pointer();

	void attach_uniform(Shader& shader, const char* uniform_name, std::vector<float>& color);
	void attach_uniform(Shader& shader, const char* uniform_name, std::vector<float>&& color);

	void bind_textures(Shader& shader, std::vector<const char*>&& uniform_names, std::vector<Texture>& texture_vector);
	void bind_textures(Shader& shader, std::vector<const char*>& uniform_names, std::vector<Texture>& texture_vector);

	void add_textures(std::vector<const char*> file_paths, std::vector<Texture>& texture_vector);

	void set_MVP(Shader& shader, Camera& camera);

	void set_color(Shader& shader, const char* uniform_name, float* color);
	void set_position(Shader& shader, Camera& camera, const char* uniform_name, float* position);

	void draw(Shader& shader, Camera& camera, unsigned int VAO, int number_of_indices, const char* uniform_color, 
		float* color, const char* uniform_position, float* position, const char* ImGui_object_name, std::function<void()> func);
	void draw(Shader& shader, Camera& camera, unsigned int VAO, int number_of_indices, const char* uniform_color,
		float* color, const char* uniform_position, float* position, const char* uniform_other_color, float* color_other,
		const char* uniform_other_position, float* position_other, const char* ImGui_object_name, std::function<void()> func);
	void draw(Shader& shader, unsigned int VAO, int number_of_indices);
	void draw(Shader& shader, unsigned int VAO, int number_of_indices, std::vector<const char*>&& uniform_names, std::vector<Texture>& texture);
	void draw(Shader& shader, unsigned int VAO, int number_of_indices, std::vector<const char*>& uniform_names, std::vector<Texture>& texture);
	void draw(Shader& shader, unsigned int VAO);
	void draw(Shader& shader, unsigned int VAO, std::vector<const char*>&& uniform_names, std::vector<Texture>& texture);
	void draw(Shader& shader, unsigned int VAO, std::vector<const char*>& uniform_names, std::vector<Texture>& texture);

	void redraw(Shader& shader, unsigned int& VBO, Mesh& new_vertices);

	const char* get_shader_type(fragment_shader_type);

	static glm::vec3 calculate_normals(glm::vec3& a, glm::vec3& b, glm::vec3& c);

	template <typename T>
	void print_data(T data) {
		std::cout << data << "\n";
	}
};

