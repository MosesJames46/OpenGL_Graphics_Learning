#include "headers/libs.h"
#include "headers/Triangle.h"
#include "headers/Square.h"
#include "headers/Shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	Shader s("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
	Triangle t;
	Square square;
	

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		t.draw(s, 0, 3);
		square.draw(s, 6);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	s.delete_program_shader();
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}