#pragma once
#include "libs.h"
#include "../extern/imgui/imgui.h"
#include "../extern/imgui/backends/imgui_impl_glfw.h"
#include "../extern/imgui/backends/imgui_impl_opengl3.h"
#include <vector>
#include <iostream>
#include <thread>
#include "Windows.h"
#include <iomanip>
#include "Shader.h"


class Camera {
public: 
	Camera(GLFWwindow* window) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	glm::vec3 camera_origin = glm::vec3{ 0.0f, 0.0f, 3.0f };
	glm::vec3 camera_forward = glm::normalize(glm::vec3{ 0.0f, 0.0f, 1.0f });
	glm::vec3 camera_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera_forward));
	glm::vec3 camera_up = glm::normalize(glm::cross(camera_forward, camera_right));

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	std::vector<GLenum> cursor = { GLFW_CURSOR_DISABLED, GLFW_CURSOR_NORMAL };

	float yaw = 90.0f;
	float pitch = 0.0f;
	float last_x_position = 960, last_y_position = 540;
	float last_frame = 0.0f;
	float last_toggle = 0.0f;

	int selector = 0;

	bool first_mouse = true;
	bool first_debug = true;
	GLenum is_edit_mode = GLFW_CURSOR_DISABLED;
	GLenum last_edit_mode = is_edit_mode;

	void get_camera_input(GLFWwindow* window);
	void mouse_callback(GLFWwindow* window, double x_position, double y_position);
	void view_through_camera(Shader& shader);

	glm::vec3 get_camera_direction(float yaw, float pitch);
};