#include "../headers/Renderer.h"
#include "../headers/Camera.h"
#include "../headers/Shader.h"
#include "../headers/Material.h"
#include "../headers/Texture.h"
#include "../headers/Mesh.h"
#include "../headers/Gui_Settings.h"
#include "../headers/Mesh_Types/Spotlight_Mesh.h"
#include "../headers/Mesh_Types/Light_Mesh.h"
#include "../headers/Mesh_Types/Complex_Mesh.h"
#include "../headers/Mesh_Types/Texture_Mesh.h"

Renderer::Renderer(std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material, Camera& camera) : mesh(std::move(mesh)),
material(std::move(material)), camera(camera) {
	initiate_renderer();
}

void Renderer::initiate_renderer() {
	gen_bind_format(*mesh.get());
	set_attributes(0, 3, 11, 0);
	set_attributes(1, 3, 11, 3);
	set_attributes(2, 2, 11, 6);
	set_attributes(3, 3, 11, 8);
	unbind_buffers_and_attribute_pointer();

	//bound another VAO for the bounding box's shaders.
	glGenVertexArrays(1, &(*mesh.get()).bounds_VAO);
	glGenBuffers(1, &(*mesh.get()).bounds_VBO);
	glGenBuffers(1, &(*mesh.get()).bounds_EBO);

	glBindVertexArray((*mesh.get()).bounds_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, (*mesh.get()).bounds_VBO);
	glBufferData(GL_ARRAY_BUFFER, (*mesh.get()).bounding_box_data.size() * sizeof(float), (*mesh.get()).bounding_box_data.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*mesh.get()).bounds_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*mesh.get()).bounding_box_indices.size() * sizeof(unsigned int), (*mesh.get()).bounding_box_indices.data(), GL_STATIC_DRAW);

	set_attributes(0, 3, 3, 0);
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
void Renderer::generate_and_bind_buffers(Mesh& mesh) {
	glGenVertexArrays(1, &mesh.VAO);
	glGenBuffers(1, &mesh.VBO);
	glGenBuffers(1, &mesh.EBO);

	glBindVertexArray(mesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
}

void Renderer::format_buffer(Mesh& mesh, GLenum draw_type) {
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

void Renderer::gen_bind_format(Mesh& mesh) {
	generate_and_bind_buffers(mesh);
	format_buffer(mesh, GL_STATIC_DRAW);
}

void Renderer::unbind_buffers_and_attribute_pointer() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
}

void Renderer::attach_uniform(Shader& shader, const char* uniform_name, std::vector<float>& color) {
	int uniform_location = glGetUniformLocation(shader.programShaderID, uniform_name);
	glUniform4f(uniform_location, color[0], color[1], color[2], 1.0f);
}

void Renderer::attach_uniform(Shader& shader, const char* uniform_name, std::vector<float>&& color) {
	int uniform_location = glGetUniformLocation(shader.programShaderID, uniform_name);
	shader.useProgram();
	glUniform4f(uniform_location, color[0], color[1], color[2], 1.0f);
}

void Renderer::bind_textures(Shader& shader, std::vector<const char*>& uniform_names) {
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].texture);
		shader.set_uniform_location(uniform_names[i], i);
	}
}
void Renderer::bind_textures(Shader& shader, std::vector<const char*>&& uniform_names) {
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].texture);
		shader.set_uniform_location(uniform_names[i], i);
	}
}

void Renderer::add_textures(Shader& shader, std::vector<const char*> file_paths, std::vector<const char*> uniform_locations) {
	for (int i = 0; i < file_paths.size(); i++) {
		Texture T;
		textures.push_back(T);
		textures[i].create_texture(file_paths[i]);
	}

	bind_textures(shader, uniform_locations);
}

void Renderer::set_MVP(Shader& shader, Camera& camera) {
	shader.set_uniform_location("model", camera.model);
	shader.set_uniform_location("view", camera.view);
	shader.set_uniform_location("projection", camera.projection);
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

	
	apply_shaders((*material.get()), true);
	set_MVP(*material->shader.get(), camera);
	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::apply_shaders(Material& material, bool render) {
	material.shader->useProgram();
	switch (material.material) {
	case LIGHT:
		material.apply_shader(dynamic_cast<Light_Mesh&>(*mesh.get()), render);
		break;
	case COMPLEX:
		material.apply_shader(dynamic_cast<Complex_Mesh&>(*mesh.get()), render);
		break;
	case SPOTLIGHT:
		material.apply_shader(dynamic_cast<Spotlight_Mesh&>(*mesh.get()), render);
		break;
	default:
		return;
	}
}

/*
	The highlight any object within a scene:
	1. Esnure the stencil buffer has writing enable and set to always keep for whenever the stencil fails, depth pass. But Replace if both the stencil pass and depth pass.
	2. Draw the object the first time.
	3. Disable writing to the stencil buffer.
	4. draw the same object but this time scaled up.
	5. Tell the stencil buffer to pass values if objects that are written do not equal to the values of the original stencil.
		- i.e. Since the object is scale from is original position,  we know anything outside of the original object's stencil value will be not equal to 1.
	6. disable the depth buffer to ensure the object draw after is rendered behind the original object. 
	7. enable writing to the stencil buffer.
*/

void Renderer::draw_highlights(bool renderer_gui) {
	glEnable(GL_STENCIL_TEST); //Do not assume that stencil testing is on.

	//This ensures that the stencil is always enable before proceeding with the operation.
	//glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST);	//Do not assume Depth testing is enabled.
	//glStencilOp: What actions to take if stencil test passes or fails and if depth test passes or fails.
	//glStencilOp: sfail, dpfail, dppass -> stencil fail, stencil pass but depth fails, and both pass.
	
	//By ensuring that our values get replaced when both the stencil and depth buffer pass, we write 1 where our original object would be.
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glClear(GL_STENCIL_BUFFER_BIT);
	//glStencilFunc sets parameters that determine if the stencil test will pass.
	//glStencilFunc: enum, ref, mask -> the enum is the requirement necessary to pass a stencil test. See https://learnopengl.com/Advanced-OpenGL/Stencil-testing
	//-> ref is the value that will be and against the mask. Mask are the values set to the stencil buffer.
	//The glStencilMask function tells the stencil to start processing request if 0xFF is passed and vice versa if 0x0 is passed.
	draw(renderer_gui);
	//Before we draw the object again, we need to disable the stencil buffer.

	//After the draw call, use the glStnecilFunc to check if any values are not equal.
	//Depth testing is disabled to that our newly rendered object is behind our current object.

	//This tells OpenGL that whenever the stencil value of a fragment is equal (GL_NOTEQUAL) to the reference value 1,
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilMask(0x00); //Bit in the stencil buffer becomes zero (disables writing).
	glDisable(GL_DEPTH_TEST);
	material->apply_highlight_shader(mesh.get());
	//glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST); //Makes sense to disable after using since we can not assume that it will be used again.
}

void Renderer::redraw() {
	material->shader->useProgram();
	//set_MVP();
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->vertex_data.size() * sizeof(float), mesh->vertex_data.data());
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