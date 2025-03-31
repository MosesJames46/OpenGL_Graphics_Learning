#include "../headers/Square.h"

void Square::draw(Shader shader, int number_of_indices) {
	shader.useProgram();
	glBindVertexArray(square_VAO);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
}

void Square::draw(Shader& shader, int number_of_indices, std::vector<const char*>& uniform_names) {
	shader.useProgram();
	bind_textures(shader, uniform_names);
	glBindVertexArray(square_VAO);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
}

void Square::draw(Shader& shader, int number_of_indices, std::vector<const char*>&& uniform_names) {
	shader.useProgram();
	bind_textures(shader, uniform_names);
	glBindVertexArray(square_VAO);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
}

void Square::initialize_mesh() {
	Vertex vertex_one(glm::vec3{ -0.5f, -0.5f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec2{ 0.0f, 0.0f });
	Vertex vertex_two(glm::vec3{ -0.5f, 0.5f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f, }, glm::vec2{ 0.0f, 1.0f });
	Vertex vertex_three(glm::vec3{ 0.5f, 0.5f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec2{ 1.0f, 1.0f });
	Vertex vertex_four(glm::vec3{ 0.5f, -0.5f, 0.0f }, glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec2{ 1.0f, 0.0f });
	mesh.load_vertex(vertex_one);
	mesh.load_vertex(vertex_two);
	mesh.load_vertex(vertex_three);
	mesh.load_vertex(vertex_four);
}

void Square::ready_buffers() {
	gen_bind_format(mesh, square_indices, square_VAO, square_VBO, square_EBO);
	set_attributes(0, 3, 8, 0);
	set_attributes(1, 3, 8, 3);
	set_attributes(2, 2, 8, 6);
	unbind_buffers_and_attribute_pointer();
}

void Square::bind_textures(Shader& shader, std::vector<const char*>& uniform_names) {
	if (!square_textures.empty()) {
		for (int i = 0; i < square_textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, square_textures[i].texture);
			shader.set_uniform_location(uniform_names[i], (int)i);
		}
	}
}

void Square::bind_textures(Shader& shader, std::vector<const char*>&& uniform_names) {
	if (!square_textures.empty()) {
		for (int i = 0; i < square_textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, square_textures[i].texture);
			shader.set_uniform_location(uniform_names[i], (int)i);
		}
	}
}

void Square::add_textures(std::vector<const char*> file_paths) {
	for (int i = 0; i < file_paths.size(); i++) {
		Texture T;
		square_textures.push_back(T);
		square_textures[i].create_texture(file_paths[i]);
	}
}