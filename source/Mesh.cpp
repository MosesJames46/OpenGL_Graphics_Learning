#include "../headers/Mesh.h"
#include "../headers/Sphere.h"

Mesh::Mesh(const std::string& name, shape_type shape) : name(name) {
	id = mesh_number();
	/*
		Give the Mesh a unique address. This avoids name duplicates.
	*/
	const void* n = static_cast<const void*>(this);
	std::ostringstream ss;
	ss << n;
	this->name += std::string(" " +  ss.str());
	switch (shape) {
	case SPHERE: {
		Sphere sphere(name, 16, 16, 1);
		initialize_mesh(sphere);
		break;
	}	
	}
}


void Mesh::initialize_mesh(Sphere& sphere) {
	sphere.generate_mesh((*this));
}

void Mesh::set_color() {
	//Use the starting position of the float pointer.
	ImGui::SeparatorText("Color");
	ImGui::ColorEdit3("##Color", glm::value_ptr(color));
	//We access the float values and cast them to a vec 3.
}

void Mesh::set_position() {
	float speed = 0.001f;
	if (ImGui::GetKeyName(ImGuiKey_LeftShift)) {
		speed = 0.1f;
	}
	ImGui::SeparatorText("Position");
	ImGui::DragFloat3("##Position", glm::value_ptr(position), speed, std::numeric_limits<float>::lowest(), (std::numeric_limits<float>::max)());
}

void Mesh::set_float() {
	//ImGui::DragFloat(float_name, &value, .1f, min, max);
	//shader.set_uniform_location(uniform_name, value);
}

void Mesh::set_ambient() {
	ImGui::SeparatorText("Ambient");
	ImGui::ColorEdit3("##Ambient", glm::value_ptr(ambient));
}

void Mesh::set_specular() {
	ImGui::SeparatorText("Specular");
	ImGui::ColorEdit3("##Specular", glm::value_ptr(specular));
}

void Mesh::set_shininess() {
	ImGui::Text("Shininess");
	ImGui::PushItemWidth(100);
	ImGui::DragFloat("Shininess: ", &shininess, slider_speed, .0001, 100.0f);
	ImGui::PopItemWidth();
}