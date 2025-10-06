#pragma once
#include "../Mesh.h"

class Camera;

/*
	10/4/25: 
		- Impleament textured detection logic.
*/

class Complex_Mesh : public Mesh {
public:
	Complex_Mesh(GLFWwindow* window, Camera& camera, const std::string& name, shape_type shape, bool is_textured) : camera(camera)
		, Mesh(window, name, shape), is_textured(is_textured) {}

	glm::vec3 specular{ 0.5f, 0.5f, 0.5f };
	

	Camera& camera;
	bool is_textured;

	
	void calculate_complex_material();
};