#include "../headers/Camera.h"
#include "../extern/imgui/imgui.h"
#include "../extern/imgui/backends/imgui_impl_glfw.h"
#include "../extern/imgui/backends/imgui_impl_opengl3.h"

Camera::Camera(GLFWwindow* window) : window(window) {
	glfwSetWindowUserPointer(window, this);
}

void Camera::get_camera_input(GLFWwindow* window) {
	float current_time = glfwGetTime();
	float delta_time = current_time - last_frame;
	last_frame = current_time;
	float cameraSpeed = delta_time * 2;

	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::IsKeyPressed(ImGuiKey_CapsLock) && current_time - last_toggle > .5f) {
		selector = (selector + 1) % 2;
		last_edit_mode = is_edit_mode;
		is_edit_mode = cursor[selector];
		last_toggle = current_time;
	}

	/*
		for glfwSetInputMode: window, input_value, what input_value should be now.
	*/
	if (is_edit_mode != last_edit_mode) {
		glfwSetInputMode(window, GLFW_CURSOR, is_edit_mode);
		last_edit_mode = is_edit_mode;
	}

	if (is_edit_mode == GLFW_CURSOR_NORMAL && ImGui::IsKeyPressed(ImGuiKey_CapsLock)) {
		first_mouse = true;
	}

	if (is_edit_mode == GLFW_CURSOR_NORMAL) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (!io.WantCaptureKeyboard && is_edit_mode == GLFW_CURSOR_DISABLED) {
		if (ImGui::IsKeyDown(ImGuiKey_W))
			camera_origin += cameraSpeed * camera_forward;
		if (ImGui::IsKeyDown(ImGuiKey_S))
			camera_origin -= cameraSpeed * camera_forward;
		if (ImGui::IsKeyDown(ImGuiKey_A))
			camera_origin -= glm::normalize(glm::cross(camera_forward, camera_up)) * cameraSpeed;
		if (ImGui::IsKeyDown(ImGuiKey_D))
			camera_origin += glm::normalize(glm::cross(camera_forward, camera_up)) * cameraSpeed;
		if (ImGui::IsKeyDown(ImGuiKey_Space))
			camera_origin += camera_up * cameraSpeed;
		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
			camera_origin += -camera_up * cameraSpeed;
	}

	mouse_callback();
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

void Camera::mouse_callback() {
	
	ImGuiIO& io = ImGui::GetIO();

	if (!io.WantCaptureMouse && is_edit_mode == GLFW_CURSOR_DISABLED) {
		
		//Use glfw for delta values.
		glfwGetCursorPos(window, &x_position, &y_position);

		// First mouse being changed to false can cause issues if the cursor switches to disabled. Since there would be no way to  use the old positions.
		if (first_mouse) {
			last_x_position = x_position;
			last_y_position = y_position;
			first_mouse = false;
		}

		/*
			Increase x/y position based on offset values. 
			Reset the last position to the current x/y.
		*/
		float x_offset = x_position - last_x_position;
		float y_offset = last_y_position - y_position;
		last_x_position = x_position;
		last_y_position = y_position;

		float sensitivity = 0.1f;

		yaw += x_offset * sensitivity;
		pitch += y_offset * sensitivity;

		//Esnures the pitch does not go out of it's bounds.
		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		camera_forward = glm::normalize(get_camera_direction(yaw, pitch));
	}
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