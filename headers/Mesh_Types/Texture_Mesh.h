#include "../Mesh.h"

class Camera;

class Texture_Mesh : public Mesh{
public:
	Texture_Mesh(const std::string& name, shape_type shape, Camera& camera) : Mesh(name, shape), camera(camera) {}

	glm::vec3 specular{ 1.0f, 1.0f, 1.0f };
	float shininess{ 32.f };
	Camera& camera;

	void set_specular();
	void set_shininess();
};