#include "../headers/Mesh.h"
#include "../headers/Sphere.h"

Mesh::Mesh(const std::string& name, shape_type shape) : name(name) {
	id = mesh_number();
	switch (shape) {
	case shape_type::SPHERE:
		Sphere sphere(name, 16, 16, 1);
		initialize_mesh(sphere);
		return;
	}
}

void Mesh::initialize_mesh(Sphere& sphere) {
	sphere.generate_mesh((*this));
}

void Mesh::set_color() {

	ImGui::Text(name.c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(100);
	//Use the starting position of the float pointer.
	std::string cw = "Color##" + (std::string)name + std::to_string(id);
	ImGui::ColorEdit3(cw.c_str(), glm::value_ptr(color));
	ImGui::PopItemWidth();
	//We access the float values and cast them to a vec 3.
}

void Mesh::set_position() {
	float speed = 0.001f;
	if (ImGui::GetKeyName(ImGuiKey_LeftShift)) {
		speed = 0.1f;
	}

	ImGui::DragFloat3("Sphere Position", glm::value_ptr(position), speed, std::numeric_limits<float>::lowest(), (std::numeric_limits<float>::max)());
}

void Mesh::set_float() {
	//ImGui::DragFloat(float_name, &value, .1f, min, max);
	//shader.set_uniform_location(uniform_name, value);
}

void Mesh::set_ambient() {
	ImGui::Text("Ambient Value");
	std::string cw = "Ambient##" + (std::string)name + std::to_string(id);
	ImGui::ColorEdit3(cw.c_str(), glm::value_ptr(ambience));
}

void Mesh::set_shininess() {
	ImGui::Text("Shininess");
	ImGui::PushItemWidth(100);
	ImGui::DragFloat("Shininess: ", &shininess, slider_speed,.0001, 100.0f);
	ImGui::PopItemWidth();
}