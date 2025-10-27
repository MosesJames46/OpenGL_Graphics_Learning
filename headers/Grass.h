#pragma once
#include <iostream>
#include <vector>
#include "../headers/libs.h"

class Camera;
class Shader;

class Grass {
public:
	Grass() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(unsigned int), index_data.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);

		
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	};

	std::vector<float> vertex_data{
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	std::vector<unsigned int> index_data{
		0, 1, 2,
		0, 2, 3
	};

	unsigned int VAO, VBO, EBO;

	void draw_plane(Camera& camera);

private:
	glm::mat4 scale = glm::mat4(1.0f);
	float scale_array[3] = { 1.0f, 1.0f, 1.0f };

	glm::mat4 rotation = glm::mat4(1.0f);
	float rotation_array[3] = { 1.0f, 1.0f, 1.0f };

	glm::mat4 translation = glm::mat4(1.0f);
	float translation_array[3] = {1.0f, 1.0f, 1.0f};

	float uniform_scale = 1.0f;

	
};