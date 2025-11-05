#include "../headers/Cube_Highlight.h"
#include "../headers/Shader.h"
#include "../headers/Camera.h"
#include "../imgui/imgui.h"

void Cube_Highlight::draw_cube(Camera& camera, unsigned int texture) {
	if (edit_cube) {
		ImGui::Begin("Highlight");
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
		if (using_highlights) ImGui::DragFloat("Highlight Influence", &highlight_influnce, slider_speed, std::numeric_limits<float>::lowest(), 
			std::numeric_limits<float>::max());

		if (ImGui::BeginCombo("RR settings", rr_string_options[rr_index].c_str())) {
			for (int i = 0; i < rr_string_options.size(); ++i) {
				bool selected = (i == rr_index);
				if (ImGui::Selectable(rr_string_options[i].c_str(), &selected)) rr_index = i;
				if (selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		
		ImGui::DragFloat("Ratio Value", &ratio, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		ImGui::End();
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

	cube.set_uniform_location("has_normals", has_normals);

	//Data set for the skybox: camera_position needed for calculating reflection/refraction
	cube.set_uniform_location("camera_position", camera.camera_origin);
	cube.set_uniform_location("skybox", 0);

	//Set up a boolean for determining whether to use reflection or refraction.
	cube.set_uniform_location("rr_setting", rr_index);

	cube.set_uniform_location("ratio", ratio);

	//Turn on Stencil testing and set stencil operations and functionality.
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

	//After the first draw, we will turn off depth testing and change how we stencil test. We check for values not equal to our original object and highlgiht them white.
	//Depth testing must be turned off to write our values behind the original object.
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	
	//Turn Depth Test back on and Stencil Test off. Do not assume the rest of the program needs it.
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_STENCIL_TEST);
}