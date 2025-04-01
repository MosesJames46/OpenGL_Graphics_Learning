#include "headers/libs.h"
#include "headers/Triangle.h"
#include "headers/Square.h"
#include "headers/Cube.h"
#include "headers/Shader.h"
#include "headers/Texture.h"
#include "headers/Sphere.h"
#include "headers/Camera.h"


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

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1980.0f / 1080.0f, 0.1f, 100.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	Cube cube;
	cube.add_textures({ "pictures/awesomeface.png", "pictures/wall.jpg" }, cube.cube_textures);

	Sphere sphere(72, 36);
	sphere.set_radius(1.0f);
	Camera camera;
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.get_camera_input(window);
		view = glm::lookAt(camera.camera_origin, camera.camera_forward + camera.camera_origin, camera.camera_up);
		s.set_uniform_location("projection", projection);
		s.set_uniform_location("view", view);
		
		//cube.draw(s, cube.cube_VAO, cube.cube_indices.size(), {"texture_one", "texture_two"}, cube.cube_textures);
		
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3{ 0, 0, 0.0f });
		s.set_uniform_location("model", model);
		sphere.draw(s, sphere.sphere_VAO, sphere.sphere_indices.size());
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

