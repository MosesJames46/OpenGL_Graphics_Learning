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



class Sphere : public Shape {
public:
	Sphere(Shader& shader, Camera& camera) : shader(shader), camera(camera){
		const void* a = static_cast<const void*>(this);
		std::stringstream ss;
		ss << a;
		sphere_name = "Sphere: " + ss.str();
		initialize_mesh(36, 36);
		ready_buffers();
		unbind_buffers_and_attribute_pointer();
	}

	//Sphere(Shader& shader, float sectors, float stacks) : shader(shader) {
	//	const void* a = static_cast<const void*>(this);
	//	std::stringstream ss;
	//	ss << a;
	//	sphere_name = "Sphere: " + ss.str();
	//	initialize_mesh(sectors, stacks);
	//	ready_buffers();
	//	unbind_buffers_and_attribute_pointer();
	//}

	void initialize_mesh(float stacks, float sectors);

	void generate_sphere(int stacks, int sectors);
	void generate_indices(int stacks, int sectors);
	void generate_normals();
	void generate_basic_normals();
	void ready_buffers();

	void set_radius();
	void set_object_size();

	void draw(const char* uniform_color_name, const char* uniform_position_name);

	void sphere_options(const char* uniform_color_name, const char* uniform_position_name);

	Mesh sphere_mesh;
	std::string sphere_name;
	std::vector<unsigned int> sphere_indices;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<Texture> sphere_textures;
	
	unsigned int sphere_VAO, sphere_VBO, sphere_EBO;
	
	Shader shader;

	glm::vec3 color{1.0f, 1.0f, 1.0f};
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };

	Camera camera;

	float radius = 1;
	float prev_radius = radius;
	float slider_speed = 0.01f;
};