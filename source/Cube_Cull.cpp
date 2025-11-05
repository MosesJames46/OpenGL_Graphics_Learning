#include "../headers/Cube_Cull.h"
#include "../headers/Camera.h"
#include "../headers/Shader.h"
#include "../imgui/imgui.h"

void Cube_Cull::draw_cube(Camera& camera) {
	ImGui::Begin("Cube_Cull");
	if (draw_imgui) {
		if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) slider_speed = .1f;
		else { slider_speed = .01f; }
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("Uniform Scale", &uniform_scale, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		ImGui::SeparatorText("Scale");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Scale_1", &scale[0][0], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Scale_2", &scale[1][1], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Scale_3", &scale[2][2], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		ImGui::SeparatorText("Rotation");
		ImGui::DragFloat3("##Rotation", rotation_values, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		glm::mat4 model = glm::mat4(1.0f);
		rotation = glm::rotate(model, glm::radians(rotation_values[0]), glm::vec3(1, 0, 0));
		rotation = glm::rotate(rotation, glm::radians(rotation_values[1]), glm::vec3(0, 1, 0));
		rotation = glm::rotate(rotation, glm::radians(rotation_values[2]), glm::vec3(0, 0, 1));

		ImGui::SeparatorText("Translation");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Translate_1", &translation[3][0], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Translate_2", &translation[3][1], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Translate_3", &translation[3][2], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		if (ImGui::BeginCombo("Select Cull Option", opengl_cull_string[cull_index].c_str())) {
			for (int i = 0; i < opengl_cull_string.size(); ++i) {
				bool selected = (i == cull_index);
				if (ImGui::Selectable(opengl_cull_string[i].c_str(), &selected)) cull_index = i;
				if (selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Rotation Order", rotation_option_string[front_index].c_str())) {
			for (int i = 0; i < rotation_option_string.size(); ++i) {
				bool selected = (i == front_index);
				if (ImGui::Selectable(rotation_option_string[i].c_str(), &selected)) front_index = i;
				if (selected) ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::End();
	}

	Shader cube_cull("shaders/cube_vs.glsl", "shaders/cube_fs.glsl");

	cube_cull.useProgram();

	cube_cull.set_uniform_location("uniform_scale", uniform_scale);

	cube_cull.set_uniform_location("scale", scale);
	cube_cull.set_uniform_location("rotation", rotation);
	cube_cull.set_uniform_location("translation", translation);

	cube_cull.set_uniform_location("model", camera.model);
	cube_cull.set_uniform_location("view", camera.view);
	cube_cull.set_uniform_location("projection", camera.projection);

	cube_cull.set_uniform_location("has_normals", has_normals);

	glEnable(GL_CULL_FACE);
	glFrontFace(rotation_options[front_index]);
	glCullFace(opengl_cull_options[cull_index]);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDisable(GL_CULL_FACE);
}