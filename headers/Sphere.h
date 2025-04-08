#pragma once
#include "libs.h"
#include "Shapes.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Gui_Settings.h"
#include <cmath>

class Sphere : public Shape {
public:
	Sphere(Shader& shader) : shader(shader){
		initialize_mesh(36, 18);
		ready_buffers();
		unbind_buffers_and_attribute_pointer();
	}

	Sphere(Shader& shader, float sectors, float stacks) : shader(shader) {
		initialize_mesh(sectors, stacks);
		ready_buffers();
		unbind_buffers_and_attribute_pointer();
	}

	void initialize_mesh(float stacks, float sectors);
	void generate_sphere(int stacks, int sectors);
	void generate_indices(int stacks, int sectors);
	void ready_buffers();
	void set_radius();
	void set_color();
	void set_object_size();
	void set_position(glm::vec3 position);

	Mesh sphere_mesh;

	std::vector<unsigned int> sphere_indices;
	std::vector<unsigned int> top_sphere_indices;
	std::vector<float> vertices;
	std::vector<float> texture_coordinates;
	std::vector<Texture> sphere_textures;
	
	unsigned int sphere_VAO, sphere_VBO, sphere_EBO;
	
	Shader shader;

	glm::vec3 color{1.0f, 1.0f, 1.0f};

	float radius = 1;
	float prev_radius = radius;
	float slider_speed = 0.01f;
};