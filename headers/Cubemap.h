#pragma once
#include <vector>
#include <string>
#include "libs.h"
#include <iostream>

class Camera;

class Cubemap {
public:
	Cubemap();

	void draw_skybox(Camera& camera);

private:
	unsigned int VAO, VBO, EBO;
	unsigned int texture;

	float vertex_data[48]{
		1, -1, 1, 1, 0, 0,
		1, 1, 1, 0, 1, 0,
		-1, 1, 1, 0, 0, 1,
		-1, -1, 1, .5f, 0, 0,
		-1, -1, -1, 0, .5f, 0,
		-1, 1, -1, 0, 0, 0.5f,
		1, 1, -1, 1, 1, 0,
		1, -1, -1, 0, 1, 1
	};

	unsigned int index_data[36]{
		0, 2, 1, //Front
		0, 3, 2,

		0, 6, 7, //Right
		0, 1,6,

		1, 5, 6, //Top
		1, 2, 5,

		3, 5, 2, //Left
		3, 4, 5,

		7, 3, 0, //Bottom
		7, 4, 3,

		6, 4, 7, //Back 
		6, 5, 4
	};

	std::vector<std::string> texture_locations{
		"sky_cubemap/right.jpg", "sky_cubemap/left.jpg",
		"sky_cubemap/top.jpg", "sky_cubemap/bottom.jpg",
		"sky_cubemap/front.jpg", "sky_cubemap/back.jpg"
	};
};