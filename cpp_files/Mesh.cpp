#include "../headers/Mesh.h"

void Mesh::load_vertex(Vertex vertex) {
	mesh.push_back(vertex.coordinates.x);
	mesh.push_back(vertex.coordinates.y);
	mesh.push_back(vertex.coordinates.z);
	mesh.push_back(vertex.color.x);
	mesh.push_back(vertex.color.y);
	mesh.push_back(vertex.color.z);
	mesh.push_back(vertex.texture_coordinates.x);
	mesh.push_back(vertex.texture_coordinates.y);
}

void Mesh::load_vertices(std::vector<float>& vertices) {
	mesh.resize((vertices.size() / 3) * 8);
	int size = mesh.size();
	int vertices_index = 0;
	for (int i = 0; i < size; i+= 8) {
		mesh[i] = vertices[vertices_index];
		mesh[i + 1] = vertices[vertices_index + 1];
		mesh[i + 2] = vertices[vertices_index + 2];
		vertices_index += 3;
	}
}

void Mesh::load_textures(std::vector<float>& textures) {
	int size = mesh.size();
	int texture_index = 0;
	for (int i = 6; i < size; i+= 8) {
		mesh[i] = textures[texture_index];
		mesh[i + 1] = textures[texture_index + 1];
		texture_index += 2;
	}
}

void Mesh::load_vertices_and_colors(std::vector<float>& vertices) {
	mesh.resize((vertices.size() / 3) * 6);
	int size = mesh.size();
	int vertices_index = 0;
	for (int i = 0; i < size; i += 6) {
		mesh[i] = vertices[vertices_index];
		mesh[i + 1] = vertices[vertices_index + 1];
		mesh[i + 2] = vertices[vertices_index + 2];
		vertices_index += 3;
		
	}
}