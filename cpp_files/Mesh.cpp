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
	mesh.resize((vertices.size() / 3) * 9);
	int size = mesh.size();
	int vertices_index = 0;
	for (int i = 0; i < size; i+= 9) {
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

void Mesh::load_vertices_and_colors(std::vector<float>& vertices, std::vector<float>& normals) {
	mesh.resize(vertices.size() * 3);
	int size = mesh.size();
	for (int i = 0; i < size; i += 9) {
		mesh[i] = vertices[i];
		mesh[i + 1] = vertices[i + 1];
		mesh[i + 2] = vertices[i + 2];
		
		mesh[i + 6] = vertices[i + 6];
		mesh[i + 7] = vertices[i + 7];
		mesh[i + 8] = vertices[i + 8];
		//std::cout << mesh[i + 6] << " " << mesh[i + 7] << " " << mesh[i + 8];
	}
}

void Mesh::load_normals(std::vector<float>& normals) {
	int size = mesh.size();
	int normals_index = 0;
	for (int i = 6; i < size; i += 9) {
		mesh[i] = normals[normals_index];
		mesh[i + 1] = normals[normals_index + 1];
		mesh[i + 2] = normals[normals_index + 2];

		normals_index += 3;
	}
}

void Mesh::load_vertices_and_normals(std::vector<float>& vertices, std::vector<float>& normals) {
	load_vertices(vertices);
	load_normals(normals);
}
