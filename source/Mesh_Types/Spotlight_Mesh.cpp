#include "../headers/Flashlight.h"
#include "../headers/Mesh_Types/Spotlight_Mesh.h"
#include "../headers/Camera.h"
#include "../headers/Shader.h"


//Spotlight_Mesh::Spotlight_Mesh(Camera& camera, std::unique_ptr<Flashlight> flashlight, const std::string& name, shape_type shape) : camera(camera),
//flashlight(std::move(flashlight)), Light_Mesh(name, shape) {}

//Gui menu option to set values for cutoff.
void Spotlight_Mesh::set_cuttoff_radius() {
	ImGui::SeparatorText("Cutoff Radius");
	ImGui::SliderFloat("##cutoff", &cutoff_radius, 0.001f, 90.0f);
}

void Spotlight_Mesh::set_outer_cutoff_radius() {
	ImGui::SeparatorText("Outer Cutoff Radius");
	ImGui::SliderFloat("##outercutoff", &outer_cutoff_radius, 0.001f, 90.0f);
	ImGui::Text("Radius Multiplier");
	ImGui::SliderFloat("##radiusmultiplier", &radius_multiplier, 1.0f, 25.0f);
}

void Spotlight_Mesh::set_flashlight_color() {
	ImGui::SeparatorText("Flashlight Color");
	ImGui::ColorEdit3("##flashlight", glm::value_ptr(flashlight_color));
}

void Spotlight_Mesh::activate_cuttoff_mesh() {
	calculate_light_material();
	set_flashlight_color();
	set_cuttoff_radius();
	set_outer_cutoff_radius();
}