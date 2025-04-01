#pragma once
#include "libs.h"


class Camera {
public: 
	glm::vec3 camera_origin = glm::vec3{ 0.0f, 0.0f, 3.0f };
	glm::vec3 camera_forward = glm::normalize(glm::vec3{ 0.0f, 0.0f, 1.0f });
	glm::vec3 camera_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera_forward));
	glm::vec3 camera_up = glm::normalize(glm::cross(camera_forward, camera_right));
	glm::mat4 view = glm::lookAt(camera_origin, camera_origin + camera_forward, camera_up);

	void get_camera_input(GLFWwindow* window) {
		const float cameraSpeed = 0.05f; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera_origin += cameraSpeed * camera_forward;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera_origin -= cameraSpeed * camera_forward;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera_origin -= glm::normalize(glm::cross(camera_forward, camera_up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera_origin += glm::normalize(glm::cross(camera_forward, camera_up)) * cameraSpeed;
	}
};