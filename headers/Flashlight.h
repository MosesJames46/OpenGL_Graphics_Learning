#pragma once
#include "libs.h"

class Flashlight {
public:
	Flashlight();
	~Flashlight() {}
	glm::vec3 color{ 1.0f, 1.0f, 1.0f };
	glm::vec3 ambient{ .1f, .1f, .1f };
};