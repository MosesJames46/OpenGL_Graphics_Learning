#pragma once
#include "../Mesh.h"

class Camera;

class Complex_Mesh : public Mesh {
public:
	Complex_Mesh(Camera& camera, const std::string& name, shape_type shape) : camera(camera)
		, Mesh(name, shape) {}

	glm::vec3 specular{ 0.5f, 0.5f, 0.5f };
	

	Camera& camera;

	
	void calculate_complex_material();
};