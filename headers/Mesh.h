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
	void load_vertices(std::vector<float>& vertices);
	void load_vertices_and_colors(std::vector<float>& vertices, std::vector<float>& normals);
	void load_normals(std::vector<float>& normals);
	void load_vertices_and_normals(std::vector<float>& vertices, std::vector<float>& normals);
	void load_textures(std::vector<float>& textures);
	glm::vec3 calculate_normals(glm::vec3& a, glm::vec3& b, glm::vec3& c);
};