#include "../headers/Cube_Highlight.h"
#include "../headers/Shader.h"
#include "../headers/Camera.h"
#include "../imgui/imgui.h"

void Cube_Highlight::draw_cube(Camera& camera) {
	if (edit_cube) {
		if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
			slider_speed = 0.1f;
		}
		else {
			slider_speed = 0.01f;
		}
		//Scale Matrix
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("Uniform Scale", &uniform_scale, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		ImGui::SeparatorText("Scale Matrix");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Scale-1", &scale[0][0], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Scale-2", &scale[1][1], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Scale-3", &scale[2][2], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		//Rotation Matrix
		ImGui::SeparatorText("Rotation Matrix");
		ImGui::DragFloat3("##Rotation Matrix", rotation_values, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		glm::mat4 model = glm::mat4(1.0f);
		rotation = glm::rotate(model, glm::radians(rotation_values[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		rotation = glm::rotate(rotation, glm::radians(rotation_values[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		rotation = glm::rotate(rotation, glm::radians(rotation_values[2]), glm::vec3(0.0f, 0.0f, 1.0f));

		//Translation Matrix
		ImGui::SeparatorText("Translation Matrix");

		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Translate-1", &translation[3][0], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Translate-2", &translation[3][1], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Translate-3", &translation[3][2], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		ImGui::Checkbox("Using Highlights", &using_highlights);
		if (using_highlights) ImGui::DragFloat("Highlight Influence", &highlight_influnce, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	}
	//glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	

	Shader cube("shaders/cube_vs.glsl", "shaders/cube_fs.glsl");
	cube.useProgram();

	cube.set_uniform_location("uniform_scale", uniform_scale);

	cube.set_uniform_location("scale", scale);
	cube.set_uniform_location("rotation", rotation);
	cube.set_uniform_location("translation", translation);
	
	cube.set_uniform_location("model", camera.model);
	cube.set_uniform_location("view", camera.view);
	cube.set_uniform_location("projection", camera.projection);
	
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	

	Shader cube_highlight("shaders/cube_vs.glsl", "shaders/cube_highlight_fs.glsl");
	cube_highlight.useProgram(); 
	
	cube_highlight.set_uniform_location("uniform_scale", uniform_scale);

	cube_highlight.set_uniform_location("scale", scale);
	cube_highlight.set_uniform_location("rotation", rotation);
	cube_highlight.set_uniform_location("translation", translation);
	
	cube_highlight.set_uniform_location("model", camera.model);
	cube_highlight.set_uniform_location("view", camera.view);
	cube_highlight.set_uniform_location("projection", camera.projection);
	
	cube_highlight.set_uniform_location("highlight_influence", highlight_influnce);
	cube_highlight.set_uniform_location("using_highlights", using_highlights);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	
	glEnable(GL_DEPTH_TEST);
}