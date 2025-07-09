#pragma once
#include <vector>
#include <iostream>
#include "Shapes.h"
#include <../glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Sphere;

enum shape_type{SPHERE};

class Mesh : public Shape{
public:
	Mesh(const std::string& name, shape_type shape);
	~Mesh() {};
	
	void initialize_mesh(Sphere& sphere);

	void set_color();
	void set_ambient();
	void set_shininess();
	void set_position();
	void set_rotation();
	void set_float();

	static unsigned int mesh_number() {
		static unsigned int mesh_id = 0;
		return ++mesh_id;
	}

	glm::vec3 color{ 1.0f, 0.55f, 1.0f };
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };

	glm::vec3 ambience{ 0.1f, 0.1f, 0.1f };
	glm::vec3 specular{ 0.5f, 0.5f, 0.5f };

	float shininess = 32.0f;
	float slider_speed = 0.01f;

	std::vector<unsigned int> indices;

	/*
		The order should be Vertex, Color, Texture Coordinate, Normal
	*/
	std::vector<float> vertex_data;

	std::string name;

	unsigned int VAO, VBO, EBO;

private:
	unsigned int id;
};
