#pragma once
#include <iostream>
#include <vector>
#include "../headers/libs.h"
#include <sstream>
#include <string.>

class Camera;
class Shader;

/*
	The Blend Operation for translucent textures is as follows:
	1. Create texture.
	2. Enable blending
	3. Choose proper blend function.
	4. Choose desired glBlendEquation
	5. Render

	This lesson needs further fixing since textures need to first be sorted or weighted using the OIT algorithims.

	Generally we draw all opaque objects first. Proceed to sorting the transparent objects based on distance. Then draw transparent objects.
*/

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

	std::string name;

	bool edit_object = false;
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