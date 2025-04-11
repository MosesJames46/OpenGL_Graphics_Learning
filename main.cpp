#include "headers/libs.h"
#include "headers/Triangle.h"
#include "headers/Square.h"
#include "headers/Cube.h"
#include "headers/Shader.h"
#include "headers/Texture.h"
#include "headers/Sphere.h"
#include "headers/Camera.h"
#include "extern/imgui/imgui.h"
#include "extern/imgui/backends/imgui_impl_glfw.h"
#include "extern/imgui/backends/imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
#include "extern/imgui/backends/imgui_impl_win32.h"
#include <windows.h>
#include "../headers/Gui_Settings.h"
#include "../headers/Icosphere.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void disableCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = false; // Set cursor visibility to false
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void enableCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = true; // Set cursor visibility to false
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to open window.\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD.\n";
		return -1;
	}

	
	Camera camera(window);

	//Apparently, any callback I initialize after ImGui will be overwritten. Wished I knew this.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowUserPointer(window, &camera);

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		if (ImGui::GetIO().WantCaptureMouse) return;
		Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
		camera->mouse_callback(window, (float)xpos, (float)ypos);
		});

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImGui::StyleColorsDark();

	Shader standard_shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
	Shader light_shader("shaders/light_vertex_shader.glsl", "shaders/light_fragment_shader.glsl");

	Sphere sphere(standard_shader, camera, "Regular Sphere");
	Sphere light_sphere(light_shader, camera, "Light Sphere");
	light_sphere.position = glm::vec3(5, 0.0f, -2);

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);

		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.get_camera_input(window);	
		camera.view_through_camera();

		glfwPollEvents();
		
		Gui_Settings::call_new_frame();
		//Gui_Settings::gui_test();
		//Gui_Settings::apply_colors(sphere, s);
		
		//light_sphere.shader.set_uniform_location("light_position", sphere.position);
		//light_sphere.shader.set_uniform_location("light_color", sphere.color);
		light_sphere.draw("object_color", "object_position", "light_color", "light_position", sphere);
		sphere.draw("fragment_color", "object_position");
		
		
		//ImGui::Begin("Testing");
		//ImGui::ColorEdit3("color", &light_sphere.color[0]);
		//ImGui::DragFloat3("position", &light_sphere.position[0]);

		//ImGui::End();

		Gui_Settings::render_frame();
		
		glfwSwapBuffers(window);
	}
	standard_shader.delete_program_shader();
	glfwTerminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

