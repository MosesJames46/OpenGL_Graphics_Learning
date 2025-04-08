#include "../headers/Sphere.h"

float pi = 3.14159;

void Sphere::initialize_mesh(float sectors, float stacks) {
	generate_sphere(stacks, sectors);
	sphere_mesh.load_vertices_and_colors(vertices);
	generate_indices(stacks, sectors);
}

void Sphere::set_radius(float radius_input) {
	for (int i = 0; i < sphere_mesh.mesh.size(); i+= 6) {
		sphere_mesh.mesh[i] = (sphere_mesh.mesh[i] / radius) * radius_input;
		sphere_mesh.mesh[i + 1] = (sphere_mesh.mesh[i + 1] / radius) * radius_input;
		sphere_mesh.mesh[i + 2] = (sphere_mesh.mesh[i + 2] / radius) * radius_input;
	}
	radius = radius_input;
	redraw(shader, sphere_VBO, sphere_mesh);
	unbind_buffers_and_attribute_pointer();
}

void Sphere::ready_buffers() {
	gen_bind_format(sphere_mesh, sphere_indices, sphere_VAO, sphere_VBO, sphere_EBO);
	set_attributes(0, 3, 6, 0);
	set_attributes(1, 3, 6, 3);
	unbind_buffers_and_attribute_pointer();
}

void Sphere::generate_sphere(int sectors, int stacks) {
	int sector_count = sectors;
	int stack_count = stacks;
	float sector_step = 2 * pi / sector_count; //The percentage needed to complete a full 2 * pi in relation to the sector count total.
	float stack_step = pi / stack_count;// The percentage needed to complete a full pi in relation to the stack count total.
	float xy, x, y, z;
	float half_pi = (pi / 2);
	vertices.clear();
	
	/*
	float current_stack_angle = half_pi - i * stack_step;

	Obtains a percentage of half a circle.
	when i is 0 only have influence in the y.
	When i is half of our sector count, we have both x and y distances equal to unit 1.
	Only when i is equal to the stack count do we enter -half_pi which gives us influence
	in the -y direction. This effectively acts as a force from pi/2 to -pi / 2;
	*/

	for (int i = 0; i <= stack_count; i++) {
		float current_stack_angle = half_pi - i * stack_step;
		xy = radius * cosf(current_stack_angle);
		z = radius * sinf(current_stack_angle);
		for (int j = 0; j <= sector_count; j++) {
			float current_sector_angle = j * sector_step;
			x = xy * cosf(current_sector_angle);
			y = xy * sinf(current_sector_angle);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}
}

void Sphere::generate_indices(int stack, int sector) {
	sphere_indices.clear();
	int k1, k2;
	for (int i = 0; i < stack; i++) {
		k1 = i * (sector + 1);
		k2 = k1 + sector + 1;
		for (int j = 0; j < sector; j++, k1++, k2++) {
			if (i != 0) {
				sphere_indices.push_back(k1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k1 + 1);
			}

			if (i != (stack)) {
				sphere_indices.push_back(k1 + 1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k2 + 1);
			}

			if (i != stack - 1) {
				sphere_indices.push_back(k1 + 1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k2 + 1);
			}

			sphere_indices.push_back(k1);
			sphere_indices.push_back(k2);

			if (i != 0) {
				sphere_indices.push_back(k1);
			}
		}
	}
}
void Sphere::set_color() {
	//static float color = 0;
	ImGui::Begin("Testing Window");
	float speed = 0.001f; // base drag speed

	// Optional: modify speed based on key modifiers
	if (ImGui::IsKeyDown(ImGuiMod_Alt)) {
		speed = 0.0001f; // very fine control
	}
	else if (ImGui::IsKeyDown(ImGuiMod_Shift)) {
		speed = 0.025f; // coarse control
	}

	ImGui::Text("Color:");
	ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##R", &color.x, speed, 0.0f, 1.0f, "R: %.2f");
	ImGui::SameLine();
	ImGui::DragFloat("##G", &color.y, speed, 0.0f, 1.0f, "G: %.2f");
	ImGui::SameLine();
	ImGui::DragFloat("##B", &color.z, speed, 0.0f, 1.0f, "B: %.2f");
	ImGui::PopItemWidth();
	ImGui::End();

	shader.set_uniform_location("fragment_color", color);
}
void Sphere::set_object_size() {
	ImGui::Begin("Sphere");
	ImGui::SliderFloat("float", &radius, 0.1f, 10.0f);
	set_radius(radius);
	ImGui::End();
}

void Sphere::set_position(glm::vec3 position) {
	glm::mat4 mat = glm::mat4(1.0f);
	shader.set_uniform_location("model", mat);
}