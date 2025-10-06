#pragma once
#include <vector>
#include <iostream>
#include "Shapes.h"
#include <../glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <sstream>
#include <memory>

class Sphere;
class GLFWwindow;
class ImVec2;
class Camera;

enum shape_type{SPHERE, FLASHLIGHT};

/*
	10/4/25: 
		- Introduce the window to the Mesh class to allow for callbacks for mouse position.
*/

class Mesh : public Shape{
public:
	Mesh(const std::string& name, shape_type shape, Camera& camera);
	Mesh(GLFWwindow* window, const std::string& name, shape_type shape, Camera& camera);
	Mesh(shape_type shape);

	~Mesh() {};
	
	void initialize_mesh(Sphere& sphere);

	void set_color();
	void set_ambient();
	void set_position();
	void set_specular();
	void set_rotation();
	void set_shininess();

	void get_screencoordiantes();

	/*
		The NDC will contain z coordinates as well. 
	*/
	void get_NDC();

	void clip_to_worldspace();

	void UI_get_cursor_position();
	
	void set_float();

	//GUI values that are sent to the GPU: Complex
	virtual void object_calculations() {
		set_color();
		set_position();
		set_ambient();
	}

	static unsigned int mesh_number() {
		static unsigned int mesh_id = 0;
		return ++mesh_id;
	}

	glm::vec3 ambient{ 0.01f, 0.01f, 0.01f };
	glm::vec3 color{ 1.0f, 0.55f, 1.0f };
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 specular{ 1.0f, 1.0f, 1.0f };
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };

	Camera& camera;

	float shininess = 32.0f;
	float slider_speed = 0.01f;

	std::vector<unsigned int> indices;

	/*
		The order should be Vertex, Color, Texture Coordinate, Normal
	*/
	std::vector<float> vertex_data;

	std::string name;
	

	unsigned int VAO, VBO, EBO;

private:
	//Necessary for the mesh to use callback functions for needs like cursor position.
	GLFWwindow* window;
	unsigned int id;
	double x_position, y_position;
	float x_NDC, y_NDC, z_NDC;
	glm::vec4 ray_in_clipspace;
	glm::vec4 ray_in_eyespace;
	glm::vec3 ray_in_worldspace;
	
};
