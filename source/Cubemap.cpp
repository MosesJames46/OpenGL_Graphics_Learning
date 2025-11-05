#include "../headers/Cubemap.h"
#include "stb_image.h"
#include "../headers/Camera.h"
#include "../headers/Shader.h"

Cubemap::Cubemap() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int width, height, color_channels;
	unsigned char* texture_data;
	for (int i = 0; i < texture_locations.size(); ++i) {
		texture_data = stbi_load(texture_locations[i].c_str(), &width, &height, &color_channels, 0);
		if (texture_data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
			stbi_image_free(texture_data);
		}else {
			std::cout << "Failed to find " << texture_locations[i] << " file location" << std::endl;
			stbi_image_free(texture_data);
		}
	}
}

void Cubemap::draw_skybox(Camera& camera) {

	Shader skybox("shaders/cubemap_vs.glsl", "shaders/cubemap_fs.glsl");

	skybox.useProgram();
	glm::mat4 view = camera.view;
	camera.view = glm::mat4(glm::mat3(camera.view));
	
	skybox.set_uniform_location("view", camera.view);
	skybox.set_uniform_location("projection", camera.projection);

	skybox.set_uniform_location("skybox", 0);

	//This is an optimization that happens during early depth testing. This works because we set out z value for the cubemap to be our w. gl_Position = pos.xyww
	//This allows for early depth testing. However, in order to properly test, we must set the Depth function to be Less than or equal to or else we get z fighting.
	glDepthFunc(GL_LEQUAL);
	//By disabling the DepthMask we always ensure we write to the furthest option in the background.
	glDepthMask(GL_FALSE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Change to default optionis
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	camera.view = view;
	
}