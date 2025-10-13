#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Shapes.h"
#include <../glm/gtc/type_ptr.hpp>
#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"
#include <sstream>
#include <memory>

class Sphere;
class GLFWwindow;
class ImVec2;
class Camera;

enum shape_type{SPHERE, MODEL};

/*
	10/6/25:
		- Introduced the Bounding Box Struct.
	10/4/25: 
		- Introduce the window to the Mesh class to allow for callbacks for mouse position.
*/

struct BoundingBox {
	float min_x = std::numeric_limits<float>::max();
	float min_y = std::numeric_limits<float>::max();
	float min_z = std::numeric_limits<float>::max();

	float max_x = std::numeric_limits<float>::min();
	float max_y = std::numeric_limits<float>::min();
	float max_z = std::numeric_limits<float>::min();
};

class Mesh : public Shape{
public:
	Mesh(const std::string& name, shape_type shape, Camera& camera);
	Mesh(GLFWwindow* window, std::string file, const std::string& name, shape_type shape, Camera& camera);
	Mesh(GLFWwindow* window, const std::string& name, shape_type shape, Camera& camera);
	Mesh(shape_type shape);

	~Mesh() {};
	
	void initialize_mesh(Sphere& sphere);

	void set_color();
	void set_ambient();
	void set_specular();
	void set_scale() {
		ImGui::SeparatorText("Scale");
		ImGui::DragFloat("##Scale", &scale, slider_speed, 0.01, 50.0f);
		ImGui::SeparatorText("Scale Matrix");
		ImGui::DragFloat3("##Scale Matrix", scale_matrix_values, 0.01f, 50.0f);
		scale_matrix[0][0] = scale_matrix_values[0];
		scale_matrix[1][1] = scale_matrix_values[1];
		scale_matrix[2][2] = scale_matrix_values[2];
	};

	void set_translation() {
		ImGui::SeparatorText("Translation");
		ImGui::DragFloat3("##Translation", glm::value_ptr(position), slider_speed, -10000000, std::numeric_limits<float>::max());
		translation_matrix[3][0] = position[0];
		translation_matrix[3][1] = position[1];
		translation_matrix[3][2] = position[2];
	}
	
	//The rotation must update from the model matrix every frame. Else it will be continous or only update the last matrix.
	void set_rotation() {
		ImGui::SeparatorText("Rotation");
		ImGui::DragFloat3("##Rotation", rotation_matrix_values, slider_speed  * 5, -100000, 100000);
		glm::mat4 model = glm::mat4(1.0f);
		rotation_matrix = glm::rotate(model, glm::radians(rotation_matrix_values[0]), glm::vec3{1.0f, 0.0f, 0.0f});
		rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation_matrix_values[1]), glm::vec3{ 0.0f, 1.0f, 0.0f });
		rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation_matrix_values[2]), glm::vec3{ 0.0f, 0.0f,1.0f });
	}

	void object_calculations() {
		set_scale();
		set_translation();
		set_rotation();

		set_color();
	}

	void set_shininess();

	void get_screencoordiantes();

	/*
		The NDC will contain z coordinates as well. 
	*/
	void get_NDC();

	void clip_to_worldspace();

	void UI_get_bounding_box();

	void set_float();

	void create_bounding_box(glm::vec3& vertex) {
		bounds.min_x = std::min(bounds.min_x, vertex.x);
		bounds.min_y = std::min(bounds.min_y, vertex.y);
		bounds.min_z = std::min(bounds.min_z, vertex.z);
		
		bounds.max_x = std::max(bounds.max_x, vertex.x);
		bounds.max_y = std::max(bounds.max_y, vertex.y);
		bounds.max_z = std::max(bounds.max_z, vertex.z);
	}

	void UI_get_cursor_position();

	static unsigned int mesh_number() {
		static unsigned int mesh_id = 0;
		return ++mesh_id;
	}

	glm::vec3 ambient{ 0.01f, 0.01f, 0.01f };
	glm::vec3 color{ 1.0f, 0.55f, 1.0f };
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 specular{ 1.0f, 1.0f, 1.0f };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 scale_mat_values{ 1.0f, 1.0f, 1.0f };

	glm::mat4 scale_matrix = glm::mat4(1.0f);
	float scale_matrix_values[3] = {scale_matrix[0][0], scale_matrix[1][1], scale_matrix[2][2]};

	glm::mat4 translation_matrix = glm::mat4(1.0f);
	float translation_matrix_values[3] = { 0, 0 , 0 };

	glm::mat4 rotation_matrix = glm::mat4(1.0f);
	float rotation_matrix_values[3] = { 0.0f, 0.0f, 0.0f };

	inline BoundingBox& get_bounds() {
		return bounds;
	}

	Camera& camera;

	float shininess = 32.0f;
	float slider_speed = 0.01f;

	float scale = 1.0f;

	std::vector<unsigned int> indices;

	/*
		The order should be Vertex, Color, Texture Coordinate, Normal
	*/
	std::vector<float> vertex_data;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texture_coordinates;

	std::vector<float> bounding_box_data;
	std::vector<unsigned int> bounding_box_indices{
		0, 1, 1, 3, 3, 2, 2, 0,
		
		4, 5, 5, 7, 7, 6, 6, 4,
		
		0, 4, 4, 5, 5, 1, 1, 0, 

		1, 5, 5, 7, 7, 3, 3, 1, 

		2, 6
	};

	std::string name;
	
	unsigned int VAO, VBO, EBO;
	unsigned int bounds_VAO, bounds_VBO, bounds_EBO;
private:
	//Necessary for the mesh to use callback functions for needs like cursor position.
	GLFWwindow* window;
	unsigned int id;
	double x_position, y_position;
	float x_NDC, y_NDC, z_NDC;

	glm::vec4 ray_in_clipspace;
	glm::vec4 ray_in_eyespace;
	glm::vec3 ray_in_worldspace;
	glm::vec3 ray_direction;

	shape_type shape;

	BoundingBox bounds;

	glm::vec3 min_x;
	glm::vec3 max_x;

	glm::vec3 min_y;
	glm::vec3 max_y;

	glm::vec3 min_z;
	glm::vec3 max_z;

	void sphere_intersection_test();
	bool bounding_box_intersection_test();
};
