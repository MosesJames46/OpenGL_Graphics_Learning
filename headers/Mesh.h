#pragma once
#include <vector>
#include "Vertex.h"

class Mesh {
public:
	std::vector<float> mesh;
	void load_vertex(Vertex vertex) {
		mesh.push_back(vertex.coordinates.x);
		mesh.push_back(vertex.coordinates.y);
		mesh.push_back(vertex.coordinates.z);
		mesh.push_back(vertex.color.x);
		mesh.push_back(vertex.color.y);
		mesh.push_back(vertex.color.z);
		mesh.push_back(vertex.texture_coordinates.x);
		mesh.push_back(vertex.texture_coordinates.y);
	}
};