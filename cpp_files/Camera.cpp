#include "../headers/Camera.h"

void Camera::get_camera_input(GLFWwindow* window, void (*mouse_callback)(GLFWwindow*, double, double)) {
	const float cameraSpeed = 0.05f; // adjust accordingly
	float last_toggle = 0.0f;
	float current_time = glfwGetTime();
	//debug_camera(window);
	
	if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS && current_time - last_toggle > 1.0f) {
		selector = (selector + 1) % 2;
		glfwSetInputMode(window, GLFW_CURSOR, cursor[selector]);
	}
	
	if (cursor[selector] == GLFW_CURSOR_NORMAL) {
		ImGui::GetIO().WantCaptureMouse = true; 
		ImGui::GetIO().WantCaptureKeyboard = true;
	}
	else {
		ImGui::GetIO().WantCaptureMouse = false;
		ImGui::GetIO().WantCaptureKeyboard = false;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera_origin += cameraSpeed * camera_forward;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera_origin -= cameraSpeed * camera_forward;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera_origin -= glm::normalize(glm::cross(camera_forward, camera_up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera_origin += glm::normalize(glm::cross(camera_forward, camera_up)) * cameraSpeed;
	}

	//glfwSetWindowUserPointer(window, this);
	//glfwSetCursorPosCallback(window, mouse_callback);
}

glm::vec3 Camera::get_camera_direction(float yaw, float pitch) {
	camera_direction.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	camera_direction.y = sinf(glm::radians(pitch));
	camera_direction.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	return camera_direction;
}

void Camera::mouse_callback(GLFWwindow* window, double x_position, double y_position) {
	if (ImGui::GetIO().WantCaptureMouse) return;
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (camera->first_mouse) {
		camera->last_x_position = x_position;
		camera->last_y_position = y_position;
		camera->first_mouse = false;
	}

	float x_offset = x_position - camera->last_x_position;
	float y_offset = camera->last_y_position - y_position;
	camera->last_x_position = x_position;
	camera->last_y_position = y_position;

	float sensitivity = 0.1f;

	camera->yaw += x_offset * sensitivity;
	camera->pitch += y_offset * sensitivity;

	if (camera->pitch > 90.0f)
		camera->pitch = 89.0f;
	if (camera->pitch < -90.0f)
		camera->pitch = -89.0f;

	glm::vec3 direction = camera->get_camera_direction(camera->yaw, camera->pitch);
	camera->camera_forward = glm::normalize(direction);
}

void Camera::debug_camera(GLFWwindow* window) {
	ImGuiIO& io = ImGui::GetIO();

	std::cout << "\033[H";
	std::cout << "\033[J"; 
	
	if (first_debug) {
		std::cout << "Mouse Pos: " << std::setw(5) << io.MousePos.x << ", " << std::setw(5) << io.MousePos.y << std::endl;
		std::cout << "Selector Value: " << selector << std::endl;
		first_debug = false;
	}

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS || x_val != io.MousePos.x && y_val != io.MousePos.y) {
		std::cout << "Mouse Pos: " << std::setw(5) << io.MousePos.x << ", " << std::setw(5) << io.MousePos.y << std::endl
		 << "Selector Value: " << selector << std::endl;
		x_val = io.MousePos.x;
		y_val = io.MousePos.y;
	}
}