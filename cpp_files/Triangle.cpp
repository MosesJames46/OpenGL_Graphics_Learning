#include "../headers/Triangle.h"

void Triangle::draw(Shader shader, int start, int vertex_count) {
	shader.useProgram();
	glBindVertexArray(triangle_VAO);
	glDrawArrays(GL_TRIANGLES, start, vertex_count);
}
