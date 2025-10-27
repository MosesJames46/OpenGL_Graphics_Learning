#include "../headers/Grass.h"
#include "../imgui/imgui.h"
#include "../headers/Camera.h"
#include "../headers/Shader.h"
#include "../headers/Texture.h"
#include "../headers/Image.h"

void Grass::draw_plane(Camera& camera) {
	float slider_speed = 0.01f;
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) slider_speed = .1f;
	ImGui::Begin("d");
	//Scale
	ImGui::DragFloat3("Scale Array Values", scale_array, slider_speed, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), "%.2f");
	scale[0][0] = scale_array[0];
	scale[1][1] = scale_array[1];
	scale[2][2] = scale_array[2];

	ImGui::DragFloat("Uniform Scale Value", &uniform_scale, slider_speed, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), "%.2f");
	//Rotation
	ImGui::DragFloat3("Rotation Array Values", rotation_array, slider_speed, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), "%.2f");

	rotation = glm::mat4(1.0f);
	rotation = glm::rotate(rotation, glm::radians(rotation_array[0]), glm::vec3{ 1.0f, 0.0f, 0.0f });
	rotation = glm::rotate(rotation, glm::radians(rotation_array[1]), glm::vec3{ 0.0f, 1.0f, 0.0f });
	rotation = glm::rotate(rotation, glm::radians(rotation_array[2]), glm::vec3{ 0.0f, 0.0f, 1.0f });

	//Translation
	ImGui::DragFloat3("Translation Array Values", translation_array, slider_speed, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), "%.2f");

	translation[3][0] = translation_array[0];
	translation[3][1] = translation_array[1];
	translation[3][2] = translation_array[2];

	ImGui::End();	

	//Image data required for processing in stbi
	int width, height, color_channels;
	unsigned char* image_data;

	//First generate image
	image_data = stbi_load("grass.png", &width, &height, &color_channels, 0);
	GLenum color_format;

	//Select proper color format based on the color channels used.
	switch (color_channels) {
	case 1:
		color_format = GL_RED; break;
	case 3:
		color_format = GL_RGB; break;
	case 4:
		color_format = GL_RGBA; break;
	default:
		color_format = GL_RED; break;
	}

	unsigned int texture;
	glGenTextures(1, &texture);

	//Process image using glTexImage2D
	if (image_data) {
		//https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml -> documentation on glTexImage2D
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D); //Esnure's mipmap is enabled.

		//Esnure our (s, t) coordinates are clamp to edge to prevent linear interpolation on the bottom of our border from draw semi transparent values at the top.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(image_data);
	}
	else {
		std::cout << "Failed to load file." << std::endl;
		stbi_image_free(image_data);
	}
	stbi_set_flip_vertically_on_load(true);

	Shader shader("shaders/grass_vs.glsl", "shaders/grass_fs.glsl");

	shader.useProgram();

	shader.set_uniform_location("model", camera.model);
	shader.set_uniform_location("view", camera.view);
	shader.set_uniform_location("projection", camera.projection);

	shader.set_uniform_location("scale", scale);
	shader.set_uniform_location("rotation", rotation);
	shader.set_uniform_location("translation", translation);

	shader.set_uniform_location("uniform_scale", uniform_scale);

	shader.set_uniform_location("grass_texture", 0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, index_data.size(), GL_UNSIGNED_INT, 0);
}