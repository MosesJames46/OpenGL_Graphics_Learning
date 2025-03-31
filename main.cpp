#include "headers/libs.h"
#include "headers/Triangle.h"
#include "headers/Square.h"
#include "headers/Shader.h"
#include "headers/Texture.h"


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
	Triangle t("pictures/wall.jpg");
	//t.add_textures({ "pictures/awesomeface.png", "pictures/wall.jpg" });
	Square square;
	square.add_textures({ "pictures/awesomeface.png", "pictures/wall.jpg"});

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1980.0f / 1080.0f, 0.1f, 100.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		s.set_uniform_location("projection", projection);
		s.set_uniform_location("view", view);
		s.set_uniform_location("model", model);
		//t.draw(s, { "texture_one", "texture_two" });
		square.draw(s, 6, {"texture_one", "texture_two"});
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