#include "../headers/Shapes.h"

void Shape::generate_and_bind_buffers(unsigned int& uninitialized_VAO, unsigned int& uninitialized_VBO, unsigned int& uninitialized_EBO) {
	glGenVertexArrays(1, &uninitialized_VAO);
	glGenBuffers(1, &uninitialized_VBO);
	glGenBuffers(1, &uninitialized_EBO);

	glBindVertexArray(uninitialized_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, uninitialized_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uninitialized_EBO);
}

void Shape::generate_and_bind_buffers(unsigned int& uninitialized_VAO, unsigned int& uninitialized_VBO) {
	glGenVertexArrays(1, &uninitialized_VAO);
	glGenBuffers(1, &uninitialized_VBO);

	glBindVertexArray(uninitialized_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, uninitialized_VBO);
}


void Shape::format_buffer(std::vector<float>& vertex_data, GLenum draw_type) {
	glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(vertex_data[0]), vertex_data.data(), draw_type);
}

void Shape::format_buffer(Mesh& vertex_data, GLenum draw_type) {
	glBufferData(GL_ARRAY_BUFFER,  vertex_data.mesh.size() * sizeof(Vertex), vertex_data.mesh.data(), draw_type);
}

void Shape::format_buffer(std::vector<float>& vertex_data, std::vector<unsigned int>& index_data, GLenum draw_type) {
	glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(vertex_data[0]), vertex_data.data(), draw_type);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(index_data[0]), index_data.data(), draw_type);

}

void Shape::format_buffer(Mesh& vertex_data, std::vector<unsigned int>& index_data, GLenum draw_type) {
	glBufferData(GL_ARRAY_BUFFER, vertex_data.mesh.size() * sizeof(float), vertex_data.mesh.data(), draw_type);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(index_data[0]), index_data.data(), draw_type);
}

void Shape::set_attributes(int attribute_position, int attribute_size, GLenum numerical_type, GLboolean normalized, int stride_size, int offset) {
	glVertexAttribPointer(attribute_position, attribute_size, numerical_type, normalized, stride_size * sizeof(float), reinterpret_cast<void*>(offset * sizeof(float)));
	glEnableVertexAttribArray(attribute_position);
}
void Shape::set_attributes(int attribute_position, int attribute_size, int stride_size, int offset) {
	glVertexAttribPointer(attribute_position, attribute_size, GL_FLOAT, GL_FALSE, stride_size * sizeof(float), reinterpret_cast<void*>(offset * sizeof(float)));
	glEnableVertexAttribArray(attribute_position);
}

void Shape::gen_bind_format(std::vector<float>& vertices, unsigned int& VAO, unsigned int& VBO) {
	generate_and_bind_buffers(VAO, VBO);
	format_buffer(vertices, GL_STATIC_DRAW);
}

void Shape::gen_bind_format(Mesh& vertices, unsigned int& VAO, unsigned int& VBO) {
	generate_and_bind_buffers(VAO, VBO);
	format_buffer(vertices, GL_STATIC_DRAW);
}

void Shape::gen_bind_format(std::vector<float>& vertices, std::vector<unsigned int>& indices, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO) {
	generate_and_bind_buffers(VAO, VBO, EBO);
	format_buffer(vertices, indices, GL_STATIC_DRAW);
}

void Shape::gen_bind_format(Mesh& vertices, std::vector<unsigned int>& indices, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO) {
	generate_and_bind_buffers(VAO, VBO, EBO);
	format_buffer(vertices, indices, GL_STATIC_DRAW);
}

void Shape::unbind_buffers_and_attribute_pointer() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
}

void Shape::attach_uniform(Shader& shader, const char* uniform_name, std::vector<float>& color) {
	int uniform_location = glGetUniformLocation(shader.programShaderID, uniform_name);
	glUniform4f(uniform_location, color[0], color[1], color[2], 1.0f);
}

void Shape::attach_uniform(Shader& shader, const char* uniform_name, std::vector<float>&& color) {
	int uniform_location = glGetUniformLocation(shader.programShaderID, uniform_name);
	shader.useProgram();
	glUniform4f(uniform_location, color[0], color[1], color[2], 1.0f);
}

void Shape::bind_textures(Shader& shader, std::vector<const char*>& uniform_names, std::vector<Texture>& texture_vector) {
	if (!texture_vector.empty()) {
		for (int i = 0; i < texture_vector.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texture_vector[i].texture);
			shader.set_uniform_location(uniform_names[i], (int)i);
		}
	}
}

void Shape::bind_textures(Shader& shader, std::vector<const char*>&& uniform_names, std::vector<Texture>& texture_vector) {
	if (!texture_vector.empty()) {
		for (int i = 0; i < texture_vector.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texture_vector[i].texture);
			shader.set_uniform_location(uniform_names[i], (int)i);
		}
	}
}

void Shape::add_textures(std::vector<const char*> file_paths, std::vector<Texture>& texture_vector) {
	for (int i = 0; i < file_paths.size(); i++) {
		Texture T;
		texture_vector.push_back(T);
		texture_vector[i].create_texture(file_paths[i]);
	}
}

void Shape::set_MVP(Shader& shader, Camera& camera) {
	shader.set_uniform_location("model", camera.model);
	shader.set_uniform_location("view", camera.view);
	shader.set_uniform_location("projection", camera.projection);
}

void Shape::draw(Shader& shader,unsigned int VAO, int number_of_indices) {
	shader.useProgram();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
}

void Shape::set_color(Shader& shader, const char* uniform_name, float* color) {

	ImGui::Text("Color:");
	ImGui::SameLine();
	ImGui::PushItemWidth(100);
	//Use the starting position of the float pointer.
	ImGui::ColorEdit3("Color Wheel", &color[0]);
	ImGui::PopItemWidth();
	glm::vec3 c = glm::vec3{ color[0], color[1], color[2] };
	//We access the float values and cast them to a vec 3.
	shader.set_uniform_location(uniform_name, c);
}

void Shape::set_position(Shader& shader, Camera& camera, const char* uniform_name, float* position) {
	float speed = 0.001f;
	if (ImGui::GetKeyName(ImGuiKey_LeftShift)) {
		speed = 0.1f;
	}

	ImGui::DragFloat3("Sphere Position", &position[0], speed, std::numeric_limits<float>::lowest(), (std::numeric_limits<float>::max)());
	glm::vec3 p = glm::vec3{ position[0], position[1] , position[2] };
	shader.set_uniform_location(uniform_name, p);
	camera.reset_camera();
	set_MVP(shader, camera);
}

void Shape::draw(Shader& shader, Camera& camera, unsigned int VAO, int number_of_indices, const char* uniform_color, 
	float* color, const char* uniform_position, float* position, const char* ImGui_object_name, std::function<void()> func) {
	shader.useProgram();
	ImGui::Begin(ImGui_object_name);
	set_color(shader, uniform_color, color);
	set_position(shader, camera, uniform_position, position);
	func();
	ImGui::End();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
}

void Shape::draw(Shader& shader, Camera& camera, unsigned int VAO, int number_of_indices, const char* uniform_color,
	float* color, const char* uniform_position, float* position, const char* uniform_color_other, float* color_other,
	const char* uniform_position_other, float* position_other, const char* ImGui_object_name, std::function<void()> func) {
	shader.useProgram();
	ImGui::Begin(ImGui_object_name);
	set_color(shader, uniform_color, color);
	set_position(shader, camera, uniform_position, position);
	glm::vec3 c = glm::vec3{ color_other[0],color_other[1], color_other[2] };
	glm::vec3 p = glm::vec3{ position_other[0], position_other[1] , position_other[2] };
	shader.set_uniform_location(uniform_color_other, c);
	shader.set_uniform_location(uniform_position_other, p);
	func();
	ImGui::End();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
}

void Shape::draw(Shader& shader,unsigned int VAO, int number_of_indices, std::vector<const char*>& uniform_names, std::vector<Texture>& texture) {
	shader.useProgram();
	bind_textures(shader, uniform_names, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
}

void Shape::draw(Shader& shader,unsigned int VAO, int number_of_indices, std::vector<const char*>&& uniform_names, std::vector<Texture>& texture) {
	shader.useProgram();
	bind_textures(shader, uniform_names, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
}

void Shape::draw(Shader& shader, unsigned int VAO) {
	shader.useProgram();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Shape::draw(Shader& shader,unsigned int VAO, std::vector<const char*>& uniform_names, std::vector<Texture>& texture) {
	shader.useProgram();
	bind_textures(shader, uniform_names, texture);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Shape::draw(Shader& shader,unsigned int VAO, std::vector<const char*>&& uniform_names, std::vector<Texture>& texture) {
	shader.useProgram();
	bind_textures(shader, uniform_names, texture);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Shape::redraw(Shader& shader, unsigned int& VBO, Mesh& new_verices) {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, new_verices.mesh.size() * sizeof(float), new_verices.mesh.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const char* Shape::get_shader_type(fragment_shader_type shader_type) {
	switch (shader_type) {
	case 0:
		return "fragment_shader.glsl";
	case 1:
		return "light_fragment_shader.glsl";
	}
}

glm::vec3 Shape::calculate_normals(glm::vec3& a, glm::vec3& b, glm::vec3& c) {
	glm::vec3 v = b - a;
	glm::vec3 u = c - a;
	return glm::cross(v, u);
}