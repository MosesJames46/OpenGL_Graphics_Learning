#pragma once
#include "../headers/Mesh_Types/Light_Mesh.h"

/*
	The spotlight class holds all data needed to create a spotlight effect, it derives from the Mesh class since it needs
	all the functions that create GUI using IMGUI.
*/

class Camera;
class Flashlight;

class Spotlight_Mesh : public Light_Mesh {
public:
	Spotlight_Mesh(Camera& camera, const std::string& name, shape_type shape) : camera(camera), Light_Mesh(name, shape) {}
	//Spotlight_Mesh(Camera& camera, std::unique_ptr<Flashlight> flashlight, const std::string& name, shape_type shape);
	
	~Spotlight_Mesh() {};

	float cutoff_radius{ 12.0f};
	glm::vec3 flashlight_color{ 1.0f, 1.0f, 1.0f };
	glm::vec3 flashlight_ambient{ .1f, .1f, .1f };
	glm::vec3 flashlight_specular{ 1.0f, 1.0f, 1.0f };

	Camera& camera;
	//std::unique_ptr<Flashlight> flashlight;

	void set_cuttoff_radius();
	void activate_cuttoff_mesh();
	void set_flashlight_color();
};