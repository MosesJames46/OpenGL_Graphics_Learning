#include "../headers/Square.h"

void Square::draw(Shader shader, int number_of_indices) {
	shader.useProgram();
	glBindVertexArray(square_VAO);
	glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
}