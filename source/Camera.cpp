#include "../headers/Camera.h"
#include "../extern/imgui/imgui.h"
#include "../extern/imgui/backends/imgui_impl_glfw.h"
#include "../extern/imgui/backends/imgui_impl_opengl3.h"

void Camera::get_camera_input(GLFWwindow* window) {
	float current_time = glfwGetTime();
	float delta_time = current_time - last_frame;
	last_frame = current_time;
	float cameraSpeed = delta_time * 2;
	
	if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS && current_time - last_toggle > 1.0f) {
		selector = (selector + 1) % 2;
		last_edit_mode = is_edit_mode;
		is_edit_mode = cursor[selector];
		last_toggle = current_time;
	}

	if (is_edit_mode != last_edit_mode) {
		glfwSetInputMode(window, GLFW_CURSOR, is_edit_mode);
		last_edit_mode = is_edit_mode;
	}
	
	if (is_edit_mode == GLFW_CURSOR_NORMAL) {
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		ImGui::GetIO().WantCaptureKeyboard = true;
		ImGui::GetIO().WantCaptureMouse = true;
		return;
	}

	if (is_edit_mode == GLFW_CURSOR_DISABLED) {
		glfwSetCursorPos(window, last_x_position, last_y_position);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera_origin += cameraSpeed * camera_forward;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera_origin -= cameraSpeed * camera_forward;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera_origin -= glm::normalize(glm::cross(camera_forward, camera_up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera_origin += glm::normalize(glm::cross(camera_forward, camera_up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera_origin += camera_up * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			camera_origin += -camera_up * cameraSpeed;
	}
}

glm::vec3 Camera::get_camera_direction(float yaw, float pitch) { 
	glm::vec3 forward = glm::vec3(
		cosf(glm::radians(yaw)) * cosf(glm::radians(pitch)),
		sinf(glm::radians(pitch)),
		sinf(glm::radians(yaw)) * cosf(glm::radians(pitch))
	);
	camera_up = glm::normalize(glm::cross(camera_right, camera_forward));
	camera_right = glm::normalize(glm::cross(camera_forward, {0.0f, 1.0f, 0.0f}));
	return forward;
}

void Camera::mouse_callback(GLFWwindow* window, double x_position, double y_position) {
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

	camera->pitch = glm::clamp(camera->pitch, -89.0f, 89.0f);

	camera->camera_forward = glm::normalize(get_camera_direction(camera->yaw, camera->pitch));
}

void Camera::view_through_camera() {
	
	projection = glm::perspective(glm::radians(45.0f), 1980.0f / 1080.0f, .01f, 100.0f);

	view = glm::lookAt(camera_origin, camera_origin + camera_forward, camera_up);

	model = glm::mat4(1.0f);
	model = glm::translate(model, camera_origin);
	reset_camera();
}

void Camera::reset_camera() {
	model = glm::mat4(1.0f);
}