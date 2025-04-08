#pragma once
#include "libs.h"
#include <sstream>
#include <fstream>
#include <../glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>


class Shader {
public:
	unsigned int programShaderID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void useProgram();
	void transfer_from_file_to_stream(const char* vertex_file_path, const char* fragment_file_path, std::string& vertex_string,
		std::string& fragment_string, std::ifstream& vertex_file, std::ifstream& fragment_file);
	unsigned int compile_shader(GLenum shader_type, const char* source_file_path);
	std::vector<unsigned int> compile_and_check_frag_vertex_shader(const char* vertex_shader_glsl_code, const char* fragment_shader_glsl_code, int success_value);
	void get_shader_iv(unsigned int shader, GLenum compilation_type, int success_value);
	void link_program_shader(unsigned int vertex_shader, unsigned int fragment_shader);
	void delete_shader(unsigned int shader);
	void delete_program_shader();

	void set_uniform_location(const char* uniform_name, int value);
	void set_uniform_location(const char* uniform_name, float value);
	void set_uniform_location(const char* uniform_name, bool value);
	void set_uniform_location(const char* uniform_name, glm::mat4 matrix);
	void set_uniform_location(const char* uniform_name, glm::vec3 vector);
	//void setMat4(const std::string& name, glm::mat4 matrix);
	//void setVec3(const std::string& name, glm::vec3 vector);
	//void setVec4(const std::string& name, glm::vec4 vector);

};
