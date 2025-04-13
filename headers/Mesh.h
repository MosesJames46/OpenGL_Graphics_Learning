#pragma once
#include <vector>
#include "Vertex.h"
#include <iostream>


class Mesh {
public:
	std::vector<float> mesh;
	void load_vertex(Vertex vertex);
	//void load_vertices(std::vector<Vertex>& vertices);
	//void load_vertices(float vertices[]);
	void load_vertices();
	void load_vertices_and_colors();
	void load_normals();
	void load_vertices_and_normals();
	void load_textures();

	void generate_sphere(int stacks, int sectors);
	void generate_indices(int stacks, int sectors);
	void generate_normals();
	void generate_basic_normals();

	glm::vec3 color{ 1.0f, 1.0f, 1.0f };
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 rotation{ 1.0f, 1.0f, 1.0f };

	float prev_x_scale = scale.x;
	float prev_y_scale = scale.y;
	float prev_z_scale = scale.z;

	float uniform_scale = 1;
	float prev_scale = uniform_scale;
	float slider_speed = 0.01f;

	std::vector<unsigned int> indices;
	std::vector<float> vertices;
	std::vector<float> normals;
};