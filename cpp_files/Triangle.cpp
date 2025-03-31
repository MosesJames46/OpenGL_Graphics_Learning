#include "../headers/Triangle.h"

void Triangle::draw(Shader shader) {
	shader.useProgram();
	glBindVertexArray(triangle_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Triangle::draw(Shader& shader, std::vector<const char*>& uniform_names) {
	shader.useProgram();
	bind_textures(shader, uniform_names);
	glBindVertexArray(triangle_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Triangle::draw(Shader& shader, std::vector<const char*>&& uniform_names) {
	shader.useProgram();
	bind_textures(shader, uniform_names);
	glBindVertexArray(triangle_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


void Triangle::ready_buffers() {
	gen_bind_format(vertex_data, triangle_VAO, triangle_VBO);
	set_attributes(0, 3, 8, 0);
	set_attributes(1, 3, 8, 3);
	set_attributes(2, 2, 8, 6);
	unbind_buffers_and_attribute_pointer();
}

void Triangle::initialize_mesh() {
	Vertex vertex_one(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec2{ 0.0f, 0.0f });
	Vertex vertex_two(glm::vec3{ 0.5f, -0.5f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec2{ 1.0f, 0.0f });
	Vertex vertex_three(glm::vec3{ 0.0f, 0.5f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec2{ 0.5f, 1.0f });
	vertex_data.load_vertex(vertex_one);
	vertex_data.load_vertex(vertex_two);
	vertex_data.load_vertex(vertex_three);
}


void Triangle::bind_textures(Shader& shader, std::vector<const char*>& uniform_names) {
	if (!triangle_textures.empty()) {
		for (int i = 0; i < triangle_textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, triangle_textures[i].texture);
			shader.set_uniform_location(uniform_names[i], (int)i);
		}
	}
}

void Triangle::bind_textures(Shader& shader, std::vector<const char*>&& uniform_names) {
	if (!triangle_textures.empty()) {
		for (int i = 0; i < triangle_textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, triangle_textures[i].texture);
			shader.set_uniform_location(uniform_names[i], (int)i);
		}
	}
}

void Triangle::add_textures(std::vector<const char*> file_paths) {
	for (int i = 0; i < file_paths.size(); i++) {
		Texture T;
		triangle_textures.push_back(T);
		triangle_textures[i].create_texture(file_paths[i]);
	}
}