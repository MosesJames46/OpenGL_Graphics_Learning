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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	void draw_cube(Camera& camera, unsigned int texture);

	bool edit_cube = true;
private:
	unsigned int VAO, VBO, EBO;

	//float vertex_data[48]{
	//	-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	//1
	//	1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	//2
	//	1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,		//3
	//	-1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.0f,	//4
	//	1.0f, -1.0f, -1.0f, 0.5f, 0.0f, 0.5f,	//5
	//	1.0f, 1.0f, -1.0f, 0.0f, 0.5f, 0.5f,	//6
	//	-1.0f, 1.0f, -1.0f, 0.5f, 0.5f, 0.5f,	//7
	//	-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f	//8
	//};

	float vertex_data[216]{
		-1, -1, 1, 0, 0, 1, 0, 0, 1, //Front
		1, -1, 1, 0, 0, 1, 0, 0, 1,
		1, 1, 1, 0, 0, 1, 0, 0, 1,
		-1, 1, 1, 0, 0, 1, 0, 0, 1,

		1, -1, 1, 0, 1, 0, 1, 0, 0, //Right
		1, -1, -1, 0, 1, 0, 1, 0, 0,
		1, 1, -1, 0, 1, 0, 1, 0, 0, 
		1, 1, 1, 0, 1, 0, 1, 0, 0,

		1, 1, 1, 1, 0, 0, 0, 1, 0, //Top
		1, 1, -1, 1, 0, 0, 0, 1, 0,
		-1, 1, -1, 1, 0, 0, 0, 1, 0,
		-1, 1, 1, 1, 0, 0, 0, 1, 0,

		-1, -1, -1, 1, 1, 0, -1, 0, 0, //Left
		-1, -1, 1, 1, 1, 0, -1, 0, 0,
		-1, 1, 1, 1, 1, 0, -1, 0, 0,
		-1, 1, -1, 1, 1, 0, -1, 0, 0, 

		-1, -1, -1, 0, 1, 1, 0, -1, 0, //Bottom
		1, -1, -1, 0, 1, 1, 0, -1, 0,
		1, -1, 1, 0, 1, 1, 0, -1, 0, 
		-1, -1, 1, 0, 1, 1, 0, -1, 0,

		1, -1, -1, 1, 0, 1, 0, 0, -1, //Back
		-1, -1, -1, 1, 0, 1, 0, 0, -1,
		-1, 1, -1, 1, 0, 1, 0, 0, -1,
		1, 1, -1, 1, 0, 1, 0, 0, -1
	};

	unsigned int index_data[36]{
		0, 1, 2,	//T1 front
		0, 2, 3,	//T2 front

		4, 5, 6,	//T3 right
		4, 6, 7,	//T4 right

		8, 9, 10,	//T5 top
		8, 10, 11,	//T6 top

		12, 13, 14,	//T7 left
		12, 14, 15,	//T8 left

		16, 17, 18,	//T9 bottom
		16, 18, 19,	//T10 bottom

		20, 21, 22,	//T11 back
		20, 22, 23		//T12 back
	};

	float slider_speed = 0.01f;

	float uniform_scale = 1.0f;
	glm::mat4 scale = glm::mat4(1.0f);

	glm::mat4 rotation = glm::mat4(1.0f);
	float rotation_values[3] = { 0.0f, 0.0f, 0.0f };

	glm::mat4 translation = glm::mat4(1.0f);

	float highlight_influnce = 1.01f;
	bool using_highlights = false;

	bool has_normals = true;

	int rr_index = 0;
	std::vector<std::string> rr_string_options{ "reflect", "refract" };
	enum rr_settings{REFLECT, REFRACT};
	float ratio = 1.0f;
};