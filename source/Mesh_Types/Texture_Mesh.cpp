#include "../headers/Mesh_Types/Texture_Mesh.h"
#include "../headers/Camera.h"

void Texture_Mesh::set_specular() {
	ImGui::SeparatorText("Specular");
	ImGui::DragFloat3("##specular", glm::value_ptr(specular), slider_speed, 0.01f, 1.0f);
}

void Texture_Mesh::set_shininess() {
	ImGui::SeparatorText("Shininess");
	ImGui::DragFloat("##shiny", &shininess, slider_speed, 0.01, 100.0f);
}