#include "../headers/Sphere.h"

float pi = 3.14159;

void Sphere::initialize_mesh(float sectors, float stacks) {
	sphere_mesh.generate_sphere(stacks, sectors);
	sphere_mesh.generate_indices(stacks, sectors);
	sphere_mesh.generate_basic_normals();
	//generate_normals();
	sphere_mesh.load_vertices_and_normals();
}

void Sphere::set_radius() {
	if (sphere_mesh.prev_scale == sphere_mesh.uniform_scale) return;
	for (int i = 0; i < sphere_mesh.mesh.size(); i+= 9) {
		sphere_mesh.mesh[i] = (sphere_mesh.mesh[i] / sphere_mesh.prev_scale) * sphere_mesh.uniform_scale;
		sphere_mesh.mesh[i + 1] = (sphere_mesh.mesh[i + 1] / sphere_mesh.prev_scale) * sphere_mesh.uniform_scale;
		sphere_mesh.mesh[i + 2] = (sphere_mesh.mesh[i + 2] / sphere_mesh.prev_scale) * sphere_mesh.uniform_scale;
	}

	sphere_mesh.prev_scale = sphere_mesh.uniform_scale;
	redraw(shader, sphere_VBO, sphere_mesh);
	unbind_buffers_and_attribute_pointer();
}

void Sphere::set_scale() {
	if (sphere_mesh.prev_x_scale == sphere_mesh.scale.x && sphere_mesh.prev_y_scale == sphere_mesh.scale.y && sphere_mesh.prev_z_scale == sphere_mesh.scale.z) return;

	for (int i = 0; i < sphere_mesh.mesh.size(); i+=9) {
		sphere_mesh.mesh[i] = (sphere_mesh.mesh[i] / sphere_mesh.prev_x_scale) * sphere_mesh.scale.x;
		sphere_mesh.mesh[i + 1] = (sphere_mesh.mesh[i + 1] / sphere_mesh.prev_y_scale) * sphere_mesh.scale.y;
		sphere_mesh.mesh[i + 2] = (sphere_mesh.mesh[i + 2] / sphere_mesh.prev_z_scale) * sphere_mesh.scale.z;
	}

	sphere_mesh.prev_x_scale = sphere_mesh.scale.x;
	sphere_mesh.prev_y_scale = sphere_mesh.scale.y;
	sphere_mesh.prev_z_scale = sphere_mesh.scale.z;
	redraw(shader, sphere_VBO, sphere_mesh);
	unbind_buffers_and_attribute_pointer();
}

void Sphere::ready_buffers() {
	gen_bind_format(sphere_mesh, sphere_mesh.indices, sphere_VAO, sphere_VBO, sphere_EBO);
	set_attributes(0, 3, 9, 0);
	set_attributes(1, 3, 9, 3);
	set_attributes(2, 3, 9, 6);
	unbind_buffers_and_attribute_pointer();
}

void Sphere::set_object_size() {
	ImGui::Text("Radius: ");
	ImGui::PushItemWidth(60);
	ImGui::SameLine();
	ImGui::DragFloat("##float", &sphere_mesh.uniform_scale, 0.01f, 0.1f, (std::numeric_limits<float>::max)());
	ImGui::PopItemWidth();

	set_radius();
}

void Sphere::set_object_scale() {
	ImGui::Text("Scale:");
	ImGui::PushItemWidth(100);
	ImGui::SameLine();
	ImGui::DragFloat3("X, Y, Z", &sphere_mesh.scale[0], .01f, 0.001f, (std::numeric_limits<float>::max)());
	ImGui::PopItemWidth();
	if (sphere_mesh.scale.x < 0.001f) sphere_mesh.scale.x = 1.0f;
	if (sphere_mesh.scale.y < 0.001f) sphere_mesh.scale.y = 1.0f;
	if (sphere_mesh.scale.z < 0.001f) sphere_mesh.scale.z = 1.0f;
	
	set_scale();
}

void Sphere::draw(Sphere& sphere) {
	Shape::draw(*this, sphere, sphere_name.c_str());
}

void Sphere::draw() {
	Shape::draw(*this, sphere_name.c_str());
}