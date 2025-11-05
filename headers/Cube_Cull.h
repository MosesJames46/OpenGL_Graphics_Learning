#pragma once
#include <iostream>
#include <vector>
#include "libs.h"
#include <string>

class Camera;

class Cube_Cull {
public:

	Cube_Cull() {
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

private:
	float slider_speed = .01f;
	bool draw_imgui = true;

	unsigned int VAO, VBO, EBO;

	float vertex_data[48]{
		1, -1, 1, 1, 0, 0,
		1, 1, 1, 0, 1, 0,
		-1, 1, 1, 0, 0, 1,
		-1, -1, 1, .5f, 0, 0,
		-1, -1, -1, 0, .5f, 0,
		-1, 1, -1, 0, 0, 0.5f,
		1, 1, -1, 1, 1, 0,
		1, -1, -1, 0, 1, 1
	};

	unsigned int index_data[36]{
		0, 2, 1, //Front
		0, 3, 2,
		
		0, 6, 7, //Right
		0, 1,6,

		1, 5, 6, //Top
		1, 2, 5,

		3, 5, 2, //Left
		3, 4, 5, 

		7, 3, 0, //Bottom
		7, 4, 3,

		6, 4, 7, //Back 
		6, 5, 4
	};

	float uniform_scale = 1;
	
	glm::mat4 scale = glm::mat4(1.0f);

	glm::mat4 rotation = glm::mat4(1);
	float rotation_values[3]{ 0, 0, 0 };

	glm::mat4 translation = glm::mat4(1);

	int cull_index = 0;
	std::vector<std::string> opengl_cull_string{ "GL_FRONT", "GL_BACK", "GL_FRONT_AND_BACK" };
	std::vector<GLenum> opengl_cull_options{ GL_FRONT, GL_BACK, GL_FRONT_AND_BACK };

	int front_index = 0;
	std::vector<std::string> rotation_option_string{ "GL_CW", "GL_CCW" };
	std::vector<GLenum> rotation_options{ GL_CW, GL_CCW };

	bool has_normals = false;
};