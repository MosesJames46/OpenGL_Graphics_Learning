#pragma once
#include <iostream>
#include <vector>
#include "../headers/libs.h"

class Camera;
class Shader;

class Grass {
public:
	Grass();

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

	unsigned int texture;
};