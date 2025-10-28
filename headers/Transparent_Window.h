#pragma once
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "libs.h"
#include <memory>

class Camera;
class Shader;

class Transparent_Window {
public:
	Transparent_Window();

	void draw_transparent_window(Camera& camera);
	bool edit_object = false;
	std::string name;
private:
	unsigned int VAO, VBO, EBO;
	unsigned int texture;

	float vertex_data[20]{
		-1.f, -1.f, 0, 0.0f, 0.0f,
		1.f, -1.f, 0, 1.0f, 0.0f,
		1.f, 1.f, 0.f, 1.f, 1.f,
		-1.f, 1.f, 0.f, 0.f, 1.f
	};

	unsigned int index_data[6]{
		0, 1, 2,
		0, 2, 3
	};
	

	float uniform_scale = 1.f;

	glm::mat4 scale = glm::mat4(1.f);
	float scale_array[3] = {1, 1, 1};

	glm::mat4 rotate = glm::mat4(1.f);
	float rotate_array[3] = {};

	glm::mat4 translate = glm::mat4(1.f);
	float translate_array[3] = {};

	float slider_speed = .01f;

	std::unique_ptr<Shader> shader;

	std::vector<std::string> opengl_blend_string{
		"GL_ZERO", "GL_ONE", "GL_SRC_COLOR", "GL_ONE_MNIUS_SRC_COLOR", "GL_DST_COLOR", "GL_ONE_MINUS_DST_COLOR", "GL_SRC_ALPHA", 
		"GL_ONE_MINUS_SRC_ALPHA", "GL_DST_ALPHA", "GL_ONE_MINUS_DST_ALPA", "GL_CONSTANT_COLOR", "GL_ONE_MINUS_CONSTANT_COLOR",
		"GL_CONSTANT_ALPHA", "GL_ONE_MINUS_CONSTANT_ALHPA", "GL_SRC_ALPHA_SATURATE",
	};

	std::vector<GLenum> opengl_blend_options{ GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, 
		GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, 
		GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE };

	int source_index = 0;
	int destination_index = 0;
};