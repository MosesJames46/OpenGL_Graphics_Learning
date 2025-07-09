#include "../headers/Renderer.h"
#include "../headers/Camera.h"
#include "../headers/Shader.h"
#include "../headers/Material.h"
#include "../headers/Texture.h"
#include "../headers/Mesh.h"
#include "../headers/Gui_Settings.h"

void Renderer::generate_and_bind_buffers(unsigned int& uninitialized_VAO, unsigned int& uninitialized_VBO, unsigned int& uninitialized_EBO) {
	glGenVertexArrays(1, &uninitialized_VAO);
	glGenBuffers(1, &uninitialized_VBO);
	glGenBuffers(1, &uninitialized_EBO);

	glBindVertexArray(uninitialized_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, uninitialized_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uninitialized_EBO);
}
void Renderer::generate_and_bind_buffers(unsigned int& uninitialized_VAO, unsigned int& uninitialized_VBO) {
	glGenVertexArrays(1, &uninitialized_VAO);
	glGenBuffers(1, &uninitialized_VBO);

	glBindVertexArray(uninitialized_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, uninitialized_VBO);
}
void Renderer::generate_and_bind_buffers() {
	glGenVertexArrays(1, &mesh.VAO);
	glGenBuffers(1, &mesh.VBO);
	glGenBuffers(1, &mesh.EBO);

	glBindVertexArray(mesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
}

void Renderer::format_buffer(GLenum draw_type) {
	glBufferData(GL_ARRAY_BUFFER, mesh.vertex_data.size() * sizeof(float), mesh.vertex_data.data(), draw_type);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), draw_type);
}

void Renderer::set_attributes(int attribute_position, int attribute_size, GLenum numerical_type, GLboolean normalized, int stride_size, int offset) {
	glVertexAttribPointer(attribute_position, attribute_size, numerical_type, normalized, stride_size * sizeof(float), reinterpret_cast<void*>(offset * sizeof(float)));
	glEnableVertexAttribArray(attribute_position);
}
void Renderer::set_attributes(int attribute_position, int attribute_size, int stride_size, int offset) {
	glVertexAttribPointer(attribute_position, attribute_size, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), reinterpret_cast<void*>(offset * sizeof(float)));
	glEnableVertexAttribArray(attribute_position);
}

void Renderer::gen_bind_format() {
	generate_and_bind_buffers();
	format_buffer(GL_STATIC_DRAW);
}

void Renderer::unbind_buffers_and_attribute_pointer() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
}

void Renderer::attach_uniform(const char* uniform_name, std::vector<float>& color) {
	int uniform_location = glGetUniformLocation(material.info.shader->programShaderID, uniform_name);
	glUniform4f(uniform_location, color[0], color[1], color[2], 1.0f);
}

void Renderer::attach_uniform(const char* uniform_name, std::vector<float>&& color) {
	int uniform_location = glGetUniformLocation(material.info.shader->programShaderID, uniform_name);
	material.info.shader->useProgram();
	glUniform4f(uniform_location, color[0], color[1], color[2], 1.0f);
}

void Renderer::bind_textures(std::vector<const char*>& uniform_names, std::vector<Texture>& texture_vector) {
	if (!texture_vector.empty()) {
		for (int i = 0; i < texture_vector.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texture_vector[i].texture);
			material.info.shader->set_uniform_location(uniform_names[i], (int)i);
		}
	}
}
void Renderer::bind_textures(std::vector<const char*>&& uniform_names, std::vector<Texture>& texture_vector) {
	if (!texture_vector.empty()) {
		for (int i = 0; i < texture_vector.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texture_vector[i].texture);
			material.info.shader->set_uniform_location(uniform_names[i], (int)i);
		}
	}
}

void Renderer::add_textures(std::vector<const char*> file_paths, std::vector<Texture>& texture_vector) {
	for (int i = 0; i < file_paths.size(); i++) {
		Texture T;
		texture_vector.push_back(T);
		texture_vector[i].create_texture(file_paths[i]);
	}
}

void Renderer::set_MVP() {
	material.info.shader->set_uniform_location("model", camera.model);
	material.info.shader->set_uniform_location("view", camera.view);
	material.info.shader->set_uniform_location("projection", camera.projection);
}

void Renderer::draw() {
	material.info.shader->useProgram();
	if (material.info.camera) {
		(material.*material.activate_material((*material.info.camera)))((*material.info.camera));
	}
	else {
		(material.*material.activate_material())();
	}
	
	set_MVP();
	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::redraw() {
	material.info.shader->useProgram();
	set_MVP();
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.vertex_data.size() * sizeof(float), mesh.vertex_data.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const char* Renderer::get_shader_type(fragment_shader_type shader_type) {
	switch (shader_type) {
	case STANDARD_SHADER:
		return "fragment_shader.glsl";
	case LIGHT_SHADER:
		return "light_fragment_shader.glsl";
	}
}