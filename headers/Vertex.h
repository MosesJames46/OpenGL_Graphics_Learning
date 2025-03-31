#pragma once
#include "../headers/libs.h"

class Vertex {
public:
	Vertex(glm::vec3 coordinates, glm::vec3 color, glm::vec2 texture_coordinates) : coordinates(coordinates), color(color), 
		texture_coordinates(texture_coordinates) {};

	glm::vec3 coordinates;
	glm::vec3 color;
	glm::vec2 texture_coordinates;
};