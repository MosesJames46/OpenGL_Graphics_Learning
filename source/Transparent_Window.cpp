#include "../headers/Transparent_Window.h"
#include "stb_image.h"
#include "../imgui/imgui.h"
#include "../headers/Camera.h"
#include "../headers/Shader.h"


Transparent_Window::Transparent_Window() {
	const void* address_name = reinterpret_cast<const void*>(this);
	std::ostringstream address_stream;
	address_stream << address_name;
	name = "Transparent_Window Module" +  address_stream.str();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int width, height, color_channels;
	unsigned char* image_data;

	image_data = stbi_load("blending_transparent_window.png", &width, &height, &color_channels, 0);

	if (image_data) {
		GLenum format;
		switch (color_channels) {
		case 1:
			format = GL_RED; break;
		case 3:
			format = GL_RGB; break;
		case 4:
			format = GL_RGBA; break;
		default:
			format = GL_RGB; break;
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_set_flip_vertically_on_load(true);
		stbi_image_free(image_data);
	}else {
		std::cout << "Could not process image." << std::endl;
		stbi_image_free(image_data);
	}

	std::unique_ptr<Shader> s = std::make_unique<Shader>("shaders/grass_vs.glsl", "shaders/grass_fs.glsl");
	shader = std::move(s);
}

void Transparent_Window::draw_transparent_window(Camera& camera) {
	if (edit_object) {
		if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) slider_speed = .1f;
		
		ImGui::Begin(name.c_str());
		//Uniform Scale
		ImGui::DragFloat("Uniform Scale", &uniform_scale, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.2f");

		//Scale Matrix
		ImGui::DragFloat3("Scale Matrix", scale_array, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.2f");
		scale[0][0] = scale_array[0];
		scale[1][1] = scale_array[1];
		scale[2][2] = scale_array[2];

		//Rotation Matrix
		ImGui::DragFloat3("Rotation Matrix", rotate_array, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.2f");
		rotate = glm::mat4(1.0f);
		rotate = glm::rotate(rotate, glm::radians(rotate_array[0]), glm::vec3(1, 0, 0));
		rotate = glm::rotate(rotate, glm::radians(rotate_array[1]), glm::vec3(0, 1, 0));
		rotate = glm::rotate(rotate, glm::radians(rotate_array[2]), glm::vec3(0, 0, 1));

		//Translation Matrix
		ImGui::DragFloat3("Translation Matrix", translate_array, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.2f");
		translate[3][0] = translate_array[0];
		translate[3][1] = translate_array[1];
		translate[3][2] = translate_array[2];

		if (ImGui::BeginCombo("Source Option", opengl_blend_string[source_index].c_str())) {
			std::string blend_value_preview = opengl_blend_string[source_index];
			for (int i = 0; i < opengl_blend_string.size(); ++i) {
				bool selected = (source_index == i);
				if (ImGui::Selectable(opengl_blend_string[i].c_str(), &selected)) source_index = i;
				if (selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		
		if (ImGui::BeginCombo("Destination Options", opengl_blend_string[destination_index].c_str(), ImGuiComboFlags_WidthFitPreview)) {
			std::string destination_preview = opengl_blend_string[destination_index];
			for (int i = 0; i < opengl_blend_string.size(); ++i) {
				bool selected = (destination_index == i);
				if (ImGui::Selectable(opengl_blend_string[i].c_str(), &selected)) destination_index = i;
				if (selected)  ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}


		ImGui::End();
	}

	shader->useProgram();
	
	shader->set_uniform_location("model", camera.model);
	shader->set_uniform_location("view", camera.view);
	shader->set_uniform_location("projection", camera.projection);
	shader->set_uniform_location("uniform_scale", uniform_scale);

	shader->set_uniform_location("scale", scale);
	shader->set_uniform_location("rotation", rotate);
	shader->set_uniform_location("translation", translate);
	
	shader->set_uniform_location("grass_texture", 0);

	//See Grass.cpp for information on blending

	glEnable(GL_BLEND);

	glBlendFunc(opengl_blend_options[source_index], opengl_blend_options[destination_index]);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);
}