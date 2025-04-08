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
	void load_vertices_and_colors(std::vector<float>& vertices);
	void load_textures(std::vector<float>& textures);
};