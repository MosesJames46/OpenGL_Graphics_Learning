#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include "libs.h"

class Camera;

class Cube_Highlight {
public:
	Cube_Highlight() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void draw_cube(Camera& camera);

	bool edit_cube = true;
private:
	unsigned int VAO, VBO, EBO;

	float vertex_data[48]{
		-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//1
		1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	//2
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,		//3
		-1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.0f,	//4
		1.0f, -1.0f, -1.0f, 0.5f, 0.0f, 0.5f,	//5
		1.0f, 1.0f, -1.0f, 0.0f, 0.5f, 0.5f,	//6
		-1.0f, 1.0f, -1.0f, 0.5f, 0.5f, 0.5f,	//7
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f	//8
	};

	unsigned int index_data[36]{
		0, 1, 2,	//T1 front
		0, 2, 3,	//T2 front

		1, 4, 5,	//T3 right
		1, 5, 2,	//T4 right

		2, 5, 6,	//T5 top
		2, 6, 3,	//T6 top

		6, 3, 0,	//T7 left
		6, 0, 7,	//T8 left

		7, 0, 1,	//T9 bottom
		7, 1, 4,	//T10 bottom

		7, 4, 5,	//T11 back
		7, 5, 6		//T12 back
	};

	float slider_speed = 0.01f;

	float uniform_scale = 1.0f;
	glm::mat4 scale = glm::mat4(1.0f);

	glm::mat4 rotation = glm::mat4(1.0f);
	float rotation_values[3] = { 0.0f, 0.0f, 0.0f };

	glm::mat4 translation = glm::mat4(1.0f);

	float highlight_influnce = 1.01f;
	bool using_highlights = false;
};