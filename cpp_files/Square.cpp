#include "../headers/Square.h"


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
