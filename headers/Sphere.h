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

	void generate_sphere(int stacks, int sectors);
	void generate_indices(int stacks, int sectors);
	void generate_normals();
	void generate_basic_normals();

	void ready_buffers();

	void set_radius();
	void set_object_size();

	void draw(const char* uniform_color_name, const char* uniform_position_name);
	void draw(const char* uniform_color_name, const char* uniform_position_name,
		const char* uniform_color_name_other, const char* uniform_position_name_other, Sphere& sphere);

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

	Camera& camera;

	float radius = 1;
	float prev_radius = radius;
	float slider_speed = 0.01f;

	//std::function is a way to store memeber variable functions. This allows us to set a callback to a instantiated type and calls its function.
	//We capture the this pointer in the capture clause and call the set_object_size() of the this pointer.
	std::function<void()> func = [this]() {
		set_object_size();
		};
};