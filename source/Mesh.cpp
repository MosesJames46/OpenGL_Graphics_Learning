#include "../headers/Mesh.h"
#include "../headers/Sphere.h"
#include "../headers/libs.h"
#include "../headers/Camera.h"

Mesh::Mesh(const std::string& name, shape_type shape, Camera& camera) : name(name), camera(camera) {
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
Mesh::Mesh(GLFWwindow* window, const std::string& name, shape_type shape, Camera& camera) : Mesh(name, shape, camera) {
	this->window = window;
	//Further research is needed on this.
	if (!window) std::cout << "Window is dead" << std::endl;
	if (!this) std::cout << "The mesh is dead" << std::endl;
	glfwSetWindowUserPointer(window, this);
}


void Mesh::initialize_mesh(Sphere& sphere) {
	sphere.generate_mesh((*this));
}

void Mesh::get_screencoordiantes() {
	glfwGetCursorPos(window, &x_position, &y_position);
}

void Mesh::get_NDC() {
	/*
		https://community.khronos.org/t/how-to-get-screen-size-in-opengl/56802
		We can use an OpenGL directive to obtain the viewport since we set the callback in main.

		0 = x1 , 1 = y1, 2 =x2, 3 = y2;
	*/
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	x_NDC = ((2.f * x_position) / m_viewport[2]) - 1.f;
	y_NDC = 1.f - ((2.f * y_position) / m_viewport[3]);
	z_NDC = 1.f;
	ray_in_clipspace = glm::vec4{ x_NDC, y_NDC, -1.f, 1.f };
}

/*
	https://antongerdelan.net/opengl/raycasting.html
	Convert from clip space to world space by inversing our pipeline.
*/
void Mesh::clip_to_worldspace() {
	ray_in_eyespace = ray_in_clipspace * glm::inverse(camera.projection);
	ray_in_eyespace = glm::vec4{ ray_in_eyespace.x, ray_in_eyespace.y, -1.f, 1.f };
	ray_in_worldspace = glm::inverse(camera.view) * ray_in_eyespace;
}


void Mesh::set_color() {
	//Use the starting position of the float pointer.
	ImGui::SeparatorText("Color");
	ImGui::ColorEdit3("##Color", glm::value_ptr(color));
	//We access the float values and cast them to a vec 3.
}

void Mesh::set_position(){
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

void Mesh::set_ambient(){
	ImGui::SeparatorText("Ambient");
	ImGui::ColorEdit3("##Ambient", glm::value_ptr(ambient));
}

void Mesh::set_specular(){
	ImGui::SeparatorText("Specular");
	ImGui::ColorEdit3("##Specular", glm::value_ptr(specular));
}

void Mesh::set_shininess(){
	ImGui::Text("Shininess");
	ImGui::PushItemWidth(100);
	ImGui::DragFloat("Shininess: ", &shininess, slider_speed, .0001, 100.0f);
	ImGui::PopItemWidth();
}

void Mesh::UI_get_cursor_position() {
	static int show_cursor = 0;
	ImGuiIO& io = ImGui::GetIO();
	if (ImGui::IsKeyPressed(ImGuiKey_1)) ++show_cursor;
	if (show_cursor & 1) {
		//double x_position, y_position;
		//Use glfw for delta values.
		get_screencoordiantes();
		get_NDC();
		clip_to_worldspace();
		ImGui::SeparatorText("Cursor Position");
		ImGui::Text("Cursor Position (%g, %g)", x_position , y_position);
		ImGui::Text("Cursor in Worldspace (%g, %g, %g)", ray_in_worldspace.x, ray_in_worldspace.y, ray_in_worldspace.z);
	}
	
}