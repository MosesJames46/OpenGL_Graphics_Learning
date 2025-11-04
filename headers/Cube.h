#pragma once
#include <iostream> 
#include "../headers/libs.h"
#include <sstream>
#include <vector>
#include <string>

class Camera;

class Cube {
public:
	Cube();

	void draw_cube(Camera& camera);

	bool show_imgui = true;

private:
	std::string name;

	float vertex_data[48]{
		-1, -1, 1, 1, 0, 0, //0
		1, -1, 1, 1, 0, 0,  //1
		1, 1, 1, 1, 0, 0,	//2
		-1, 1, 1, 1, 0, 0,	//3
		1, -1, -1, 0, 1, 0,	//4
		1, 1, -1, 0, 1, 0,	//5
		-1, 1, -1, 0, 0, 1,	//6
		-1, -1, -1, 0, 0, 1	//7
	};

	//This should be a counter clockwise index order.
	unsigned int index_data[36]{
		0, 1, 2, // front
		0, 2, 3,
		1, 4, 5, // right
		1, 5, 2,
		2, 5, 6, //top
		2, 6, 3,
		7, 0, 3, // left
		7, 3, 6,
		7, 4, 1, // bottom
		7, 1, 0,
		6, 5, 4, // back
		6, 4, 7
	};

	float quad_vertices[24] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	float rear_view[24]{
		.85f, 1.f, 0.0f, 1.0f,
		.85f, .85f, 0.0f, 0.0f,
		1.f, .85f, 1.f, 0.0f,

		.85f, 1.f, 0.f, 1.0f,
		1.f, .85f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f
	};

	unsigned int VAO, VBO, EBO;

	unsigned int rear_VAO, rear_VBO, rear_texture;
	unsigned int rear_FBO, rear_RBO;

	unsigned int FBO, texture, RBO;
	unsigned int quad_VAO, quad_VBO;

	float slider_speed = .01f;

	float uniform_scale = 1.0f;

	glm::mat4 scale = glm::mat4(1.0f);
	float s[3]{ 1, 1, 1 };

	glm::mat4 rotate = glm::mat4(1.0f);
	float rotate_scalars[3]{ 0, 0, 0 };

	glm::mat4 translate = glm::mat4(1.0f);
	float t[3]{ translate[3][0], translate[3][1], translate[3][3] };

	std::vector<std::string> opengl_cullface_string_options{
		"GL_BACK", "GL_FRONT", "GL_FRONT_AND_BACK"
	};
	std::vector<GLenum> opengl_cullface_options{ GL_BACK, GL_FRONT, GL_FRONT_AND_BACK };
	int cullface_index = 0;

	std::vector<std::string> opengl_frontface_string_options{
		"GL_CCW", "GL_CW"
	};
	std::vector<GLenum> opengl_frontface_options{
		GL_CCW, GL_CW
	};
	int frontface_index = 0;

	bool using_highlight = false;
	float highlight_influence = 1.01f;

	enum shader_options{INVERT, GRAYSCALE, GRAYSCALE_AVERAGE};
	int shader_index = 0;
	std::vector<std::string> shaders{ "Invert", "Grayscale", "Grayscale Average"};

	float kernel[9]{
		1.f, 1.f, 1.f,
		1.f, 9.f, 1.f,
		1.f, 1.f, 1.f
	};

	float kernel_divisor = 1.f;
};