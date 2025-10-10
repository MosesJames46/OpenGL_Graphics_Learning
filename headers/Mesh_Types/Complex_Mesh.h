#pragma once
#include "../Mesh.h"

class Camera;

/*
	10/4/25: 
		- Impleament textured detection logic.
*/

class Complex_Mesh : public Mesh {
public:
	Complex_Mesh(GLFWwindow* window, Camera& camera, const std::string& name, shape_type shape, bool is_textured) :
	Mesh(window, name, shape, camera), is_textured(is_textured) {}
	Complex_Mesh(GLFWwindow* window, Camera& camera, std::string file, const std::string& name, shape_type shape, bool is_textured) :
		Mesh(window, file, name, shape, camera), is_textured(is_textured) {
	}

	glm::vec3 specular{ 0.5f, 0.5f, 0.5f };
	bool is_textured;

	
	void calculate_complex_material();
};