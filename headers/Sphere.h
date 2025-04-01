#pragma once
#include "libs.h"
#include "Shapes.h"
#include "Mesh.h"

class Sphere : public Shape {
public:
	Sphere() {
		initialize_mesh(36, 18);
		ready_buffers();
		unbind_buffers_and_attribute_pointer();
	}

	Sphere(float sectors, float stacks) : sectors(sectors), stacks(stacks){
		initialize_mesh(sectors, stacks);
		ready_buffers();
		unbind_buffers_and_attribute_pointer();
	}

	void initialize_mesh(float sectors, float stacks);
	void ready_buffers();
	void set_radius(float radius);

	Mesh sphere_mesh;
	std::vector<unsigned int> sphere_indices;
	std::vector<unsigned int> top_sphere_indices;
	std::vector<float> vertices;
	std::vector<float> texture_coordinates;
	unsigned int sphere_VAO, sphere_VBO, sphere_EBO;
	float radius = 1;
	float sectors, stacks;
};