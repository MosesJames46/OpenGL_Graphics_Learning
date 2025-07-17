#pragma once
#include "libs.h"
#include <vector>
#include <iostream>



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

	bool does_ImGui_have_control = true;
	void perspective(float FOV, float n, float f, float aspect, float l, float r, float b, float t, glm::vec3& in) {
		FOV = std::tanf(FOV / 2.f * (3.1415f / 180.f)) * n;
		r = aspect * FOV * r;
		l = -r;
		t = FOV * t;
		b = -t;
		glm::mat4 proj;
		in.x = in.x * proj[0][0] + in.x * proj[0][2];
		in.y = in.y * proj[1][1] + proj[1][2];
		in.z = in.z * proj[2][2] + proj[2][3];

		float w = in.x * proj[3][0] + in.y * proj[3][1] + in.z * proj[3][2] + proj[3][3];

		if (w != 0) {
			in.x /= w;
			in.y /= w;
			in.z /= w;
		}
	}

	glm::mat4 frustum(float n, float f, float fov, float aspect) {
		fov = std::tanf(fov / 2.f) * n;
		float r = aspect * fov;
		float l = -r;
		float t = fov;
		float b = -t;
		glm::mat4 frust;
		frust[0][0] = 2 * n / (r - l);
		frust[0][2] = (r + l) / (r - l);

		frust[1][1] = 2 * n / (t - b);
		frust[1][2] = t + b / (t - b);

		frust[2][2] = -(f + n) / (f - n);
		frust[2][3] = -2 * f * n / (f - n);

		frust[3][3] = -1;
		return frust;
	}

	void get_camera_input(GLFWwindow* window);
	void mouse_callback(GLFWwindow* window, double x_position, double y_position);
	void view_through_camera();
	void reset_camera();

	glm::vec3 get_camera_direction(float yaw, float pitch);
};