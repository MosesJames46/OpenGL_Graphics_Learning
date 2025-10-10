#include "../headers/Mesh_Types/Light_Mesh.h"


void Light_Mesh::set_attenuation() {
	ImGui::SeparatorText("Attenuation");
	ImGui::DragFloat3("##Quadratic", attenuation, slider_speed, .0001f, 1000.1f);
}

void Light_Mesh::calculate_light_material() {
	set_position();

	set_ambient();
	set_color();

	set_attenuation();

	UI_get_bounding_box();
	UI_get_cursor_position();
}