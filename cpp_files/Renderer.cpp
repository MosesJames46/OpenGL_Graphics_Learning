#include "../headers/Renderer.h"
#include "../headers/Camera.h"
#include "../headers/Shader.h"
#include "../headers/Material.h"
#include "../headers/Texture.h"
#include "../headers/Mesh.h"
#include "../headers/Gui_Settings.h"


Renderer::Renderer(std::unique_ptr<Material> material, Camera* camera) : unique_material(std::move(material)), camera(*camera), 
mesh((*unique_material->info.mesh)), material((*unique_material.get())) {
	//std::cout << unique_material->info.mesh->name << std::endl;
	gen_bind_format();
	set_attributes(0, 3, 11, 0);
	set_attributes(1, 3, 11, 3);
	set_attributes(2, 2, 11, 6);
	set_attributes(3, 3, 11, 8);
	unbind_buffers_and_attribute_pointer();
}

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

void Renderer::bind_textures(std::vector<const char*>& uniform_names) {
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].texture);
		material.info.shader->set_uniform_location(uniform_names[i], i);
	}
}
void Renderer::bind_textures(std::vector<const char*>&& uniform_names) {
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].texture);
		material.info.shader->set_uniform_location(uniform_names[i], i);
	}
}

void Renderer::add_textures(std::vector<const char*> file_paths, std::vector<const char*> uniform_locations) {
	for (int i = 0; i < file_paths.size(); i++) {
		Texture T;
		textures.push_back(T);
		textures[i].create_texture(file_paths[i]);
	}

	bind_textures(uniform_locations);
}

void Renderer::set_MVP() {
	material.info.shader->set_uniform_location("model", camera.model);
	material.info.shader->set_uniform_location("view", camera.view);
	material.info.shader->set_uniform_location("projection", camera.projection);
}


/*
	When invoking a pointer to functoin memeber:
	(material.*material.activate_material((*material.info.camera, render)))((*material.info.camera, render))
		1. material. looks at available memebers of the this pointer
		2.*material.activate_material dereferences the address of the poniter to function memeber
		3. the parameter list following step 2. is the parameter list needed for the function pointer.
		4. enclose all of steps 1 - 3 in parenthesis 
		5. invoke with proper parameters.
*/
void Renderer::draw(bool render) {
	material.info.shader->useProgram();
	switch (material.material) {
	case LIGHT:
		(material.*material.activate_material(render))(render);
		break;
	case COMPLEX:
		(material.*material.activate_material((*material.info.camera), render))((*material.info.camera), render);
		break;
	case TEXTURED:
		(material.*material.activate_material((*material.info.camera), render))((*material.info.camera), render);
		break;
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