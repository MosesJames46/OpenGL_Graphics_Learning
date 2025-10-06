#pragma once
#include "../Mesh.h"

class Light_Mesh : public Mesh {
public:
	/*
		Rememeber if a class is derived, it must also construct the parent/base class to work proper.
	*/
	Light_Mesh(GLFWwindow* window, const std::string& name, shape_type shape) : Mesh(window, name, shape) {};
	/*
		Calculates attenuation :
				1.0f
		---------------------------
		const + quad + linear + d^2

		d = distace;

		constant is used to ensure the values are always above 1.0f or the values would increase greatly.
	*/
	float shininess = 32.0f;
	float attenuation[3]{ 1.f, .032f, .09f };

	void set_attenuation();
	
	void calculate_light_material();
};