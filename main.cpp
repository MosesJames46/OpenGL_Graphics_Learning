#include "headers/libs.h"
#include "headers/Shader.h"
#include "headers/Material.h"
#include "headers/Sphere.h"
#include "headers/Camera.h"
#include "headers/Mesh.h"
#include "extern/imgui/imgui.h"
#include "extern/imgui/backends/imgui_impl_glfw.h"
#include "extern/imgui/backends/imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
#include "extern/imgui/backends/imgui_impl_win32.h"
#include "../headers/Gui_Settings.h"
#include "../headers/Renderer.h"
#include "../headers/Mesh_Types/Light_Mesh.h"
#include "../headers/Mesh_Types/Complex_Mesh.h"
#include "../headers/Mesh_Types/Texture_Mesh.h"
#include "../headers/Mesh_Types/Spotlight_Mesh.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, double x_position, double y_position);

constexpr const float WINDOW_WIDTH = 800.0f;
constexpr const float WINDOW_HEIGHT = 600.0f;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);

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

	
	

	//Apparently, any callback I initialize after ImGui will be overwritten. Wished I knew this.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

	/*
		When you set the callback install to true, we only let ImGui handle callbacks for OpenGL. 

		This means that obtaining input can be safely handled through ImGui.

		Important:
			When used in conjuction with glfw mouse position, it should be noted that it does not take into consideration the reset to the center of the
			screen when the mouse is disabled. This can cause confusion when switching between camera mode and input selection.
	*/
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImGui::StyleColorsDark();
	
	Gui_Settings::attach_window(window);
	Camera camera(window);
	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
		
		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		camera.get_camera_input(window);
		camera.view_through_camera();

		glfwPollEvents();
		
		Gui_Settings::call_new_frame();
		Gui_Settings::gui_test(camera);
		//Gui_Settings::gui_test_type();

		//r.draw(false);
		//r_shiny.draw(false);

		ImGui::ShowDemoWindow();

		ImGui::EndFrame();
		Gui_Settings::render_frame();
				
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

