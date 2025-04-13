#pragma once
#include "libs.h"
#include "Shapes.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Gui_Settings.h"
#include <sstream>
#include <cmath>
#include <functional>



class Sphere : public Shape {
public:
	Sphere(Shader& shader, Camera& camera, const char* name) : shader(shader), camera(camera){
		//Good tip for transforming the this pointer into a string. Static case to a const void*.
		const void* a = static_cast<const void*>(this);
		std::stringstream ss;
		ss << a;
		std::string hash = "##";
		sphere_name = name + hash + ss.str();
		initialize_mesh(36, 36);
		ready_buffers();
		unbind_buffers_and_attribute_pointer();
	}

	void initialize_mesh(float stacks, float sectors);

	void ready_buffers();

	void set_radius();
	void set_scale();

	void set_object_size();
	void set_object_scale();

	void draw(Sphere& sphere);
	void draw();

	Mesh sphere_mesh;
	std::string sphere_name;
	
	unsigned int sphere_VAO, sphere_VBO, sphere_EBO;
	
	Shader shader;

	Camera& camera;

	//std::function is a way to store memeber variable functions. This allows us to set a callback to a instantiated type and calls its function.
	//We capture the this pointer in the capture clause and call the set_object_size() of the this pointer.
};