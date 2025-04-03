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


class Camera {
public: 
	glm::vec3 camera_origin = glm::vec3{ 0.0f, 0.0f, 3.0f };
	glm::vec3 camera_forward = glm::normalize(glm::vec3{ 0.0f, 0.0f, 1.0f });
	glm::vec3 camera_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera_forward));
	glm::vec3 camera_up = glm::normalize(glm::cross(camera_forward, camera_right));
	glm::mat4 view = glm::lookAt(camera_origin, camera_origin + camera_forward, camera_up);
	glm::vec3 camera_direction = glm::vec3(0.0f, 0.0f, 0.0f);

	float yaw = 90.0f;
	float pitch = 0.0f;

	float x_val = -1, y_val = -1;
	std::vector<GLenum> cursor = { GLFW_CURSOR_NORMAL, GLFW_CURSOR_DISABLED };
	int selector = 0;

	float last_x_position = 960, last_y_position = 540;
	bool first_mouse = true;
	bool first_debug = true;

	void get_camera_input(GLFWwindow* window, void (*mouse_callback)(GLFWwindow*, double, double));
	glm::vec3 get_camera_direction(float yaw, float pitch);
	static void mouse_callback(GLFWwindow* window, double x_position, double y_position);

	void debug_camera(GLFWwindow* window);
};