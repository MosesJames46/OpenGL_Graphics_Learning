#include "../headers/Mesh.h"
#include "../headers/Sphere.h"
#include "../headers/libs.h"

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

//Just so I can remember this is called delegating constructors.
Mesh::Mesh(GLFWwindow* window, const std::string& name, shape_type shape) : Mesh(name, shape) {
	this->window = window;
	//Further research is needed on this.
	if (!window) std::cout << "Window is dead" << std::endl;
	if (!this) std::cout << "The mesh is dead" << std::endl;
	glfwSetWindowUserPointer(window, this);
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

void Mesh::UI_get_cursor_position() {
	static int show_cursor = 0;
	ImGuiIO& io = ImGui::GetIO();
	if (ImGui::IsKeyPressed(ImGuiKey_1)) ++show_cursor;
	if (show_cursor & 1 && !io.WantCaptureMouse) {
		double x_position, y_position;
		//Use glfw for delta values.
		glfwGetCursorPos(window, &x_position, &y_position);
		ImGui::Begin("##Cursuor");
		ImGui::SeparatorText("Cursor Position");
		ImGui::Text("Cursor Position (%g, %g)", x_position, y_position);
		ImGui::End();
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	
}