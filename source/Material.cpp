#include "../headers/Material.h"
#include "../headers/Mesh.h"
#include "../headers/Camera.h"
#include "../headers/Mesh_Types/Spotlight_Mesh.h"
#include "../headers/Mesh_Types/Light_Mesh.h"
#include "../headers/Mesh_Types/Complex_Mesh.h"
#include "../headers/Mesh_Types/Texture_Mesh.h"
#include "../headers/Sphere.h"
#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"
#include "../headers/Renderer.h"

Material::Material(std::unique_ptr<Shader> shader, material_type material) :
	shader(std::move(shader)), material(material), is_textured(is_textured){
	Camera cam(nullptr);
	camera_for_ray_cast = &cam;
	ray_mesh = std::make_unique<Mesh>("Ray", SPHERE, *camera_for_ray_cast);

	glGenVertexArrays(1, &ray_mesh->VAO);
	glGenBuffers(1, &ray_mesh->VBO);
	glGenBuffers(1, &ray_mesh->EBO);

	glBindVertexArray(ray_mesh->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, ray_mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, ray_mesh->vertex_data.size() * sizeof(float) , ray_mesh->vertex_data.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ray_mesh->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ray_mesh->indices.size() * sizeof(unsigned int), ray_mesh->indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
}


void Material::light_material(Light_Mesh& light_mesh, bool render) {
	//shader->set_uniform_location("light_position", light_mesh.position);
	shader->set_uniform_location("scalar", light_mesh.scale);
	shader->set_uniform_location("scale", light_mesh.scale_matrix);
	shader->set_uniform_location("light_color", light_mesh.color);
	shader->set_uniform_location("translation", light_mesh.translation_matrix);
	shader->set_uniform_location("rotation", light_mesh.rotation_matrix);

	//apply_highlight_shader(&light_mesh);

	if (render)
		light_material_data(light_mesh);
		
}

void Material::light_material_data(Light_Mesh& light_mesh) {
	ImGui::Begin(std::string(light_mesh.name).c_str());
	light_mesh.calculate_light_material();
	ImGui::End();
}

void Material::light_effects() {
	for (auto& mesh_object : mesh_objects) {
		//Light effects
		shader->set_uniform_location("light.ambient", mesh_object->ambient);
		shader->set_uniform_location("light.diffuse", mesh_object->color);

		shader->set_uniform_location("light.constant", mesh_object->attenuation[0]);
		shader->set_uniform_location("light.linear", mesh_object->attenuation[1]);
		shader->set_uniform_location("light.quadratic", mesh_object->attenuation[2]);

		shader->set_uniform_location("light.position", mesh_object->position);
	}
}

void Material::flashlight(Spotlight_Mesh* spotlight) {
	shader->set_uniform_location("light.diffuse", spotlight->flashlight_color);
	shader->set_uniform_location("light.ambient", spotlight->flashlight_ambient);
	shader->set_uniform_location("light.specular", spotlight->flashlight_specular);

	shader->set_uniform_location("light.constant", spotlight->attenuation[0]);
	shader->set_uniform_location("light.linear", spotlight->attenuation[1]);
	shader->set_uniform_location("light.quadratic", spotlight->attenuation[2]);

	shader->set_uniform_location("light.position", spotlight->camera.camera_origin);
	shader->set_uniform_location("light.cutoff", glm::cos(glm::radians(spotlight->cutoff_radius)));
	shader->set_uniform_location("light.outerCutoff", glm::cos(glm::radians(spotlight->outer_cutoff_radius)));
	shader->set_uniform_location("light.radiusMultiplier", glm::cos(glm::radians(spotlight->radius_multiplier)));

	shader->set_uniform_location("light.direction", spotlight->camera.camera_forward);

}

void Material::complex_material(Complex_Mesh& complex_mesh, bool render) {
	shader->set_uniform_location("scalar", complex_mesh.scale);
	shader->set_uniform_location("scale", complex_mesh.scale_matrix);
	shader->set_uniform_location("translate", complex_mesh.translation_matrix);
	shader->set_uniform_location("rotation", complex_mesh.rotation_matrix);
	shader->set_uniform_location("material.ambient", complex_mesh.ambient);
	shader->set_uniform_location("material.diffuse", complex_mesh.color);
	shader->set_uniform_location("material.specular", complex_mesh.specular);
	shader->set_uniform_location("material.shininess",complex_mesh.shininess);
	shader->set_uniform_location("view_position", complex_mesh.camera.camera_origin);

	if (render)
		complex_material_data(complex_mesh);
	if (mesh_objects.empty()) return;	
	light_effects();
}

void Material::complex_material_data(Complex_Mesh& complex_mesh) {
	ImGui::Begin(std::string(complex_mesh.name).c_str());
	complex_mesh.object_calculations();
	ImGui::End();
}


//Set the spotlight shader values and material data
void Material::spotlight_material(Spotlight_Mesh& spotlight, bool render) {
	shader->set_uniform_location("scalar", spotlight.scale);
	shader->set_uniform_location("scale", spotlight.scale_matrix);
	shader->set_uniform_location("translate", spotlight.translation_matrix);
	shader->set_uniform_location("rotation", spotlight.rotation_matrix);
	shader->set_uniform_location("material.ambient", spotlight.ambient);
	shader->set_uniform_location("material.diffuse", spotlight.color);
	shader->set_uniform_location("material.specular", spotlight.specular);
	shader->set_uniform_location("material.shininess", spotlight.shininess);

	shader->set_uniform_location("view_position", spotlight.camera.camera_origin);

	shader->set_uniform_location("is_textured", spotlight.is_textured);

	//apply_highlight_shader(&spotlight);
	if (render)
		spotlight_material_data(spotlight);
	flashlight(&spotlight);
}

void Material::apply_highlight_shader(Mesh* mesh) {

	stencil_shader.useProgram();
	stencil_shader.set_uniform_location("model", mesh->camera.model);
	stencil_shader.set_uniform_location("view", mesh->camera.view);
	stencil_shader.set_uniform_location("projection", mesh->camera.projection);
	stencil_shader.set_uniform_location("translation", mesh->translation_matrix);
	stencil_shader.set_uniform_location("rotation", mesh->rotation_matrix);
	stencil_shader.set_uniform_location("scale", mesh->scale_matrix);
	stencil_shader.set_uniform_location("object_position", mesh->position);
	stencil_shader.set_uniform_location("scalar", mesh->scale);

	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

}

void Material::apply_bounds_shader(Mesh* mesh) {
	bounding_box_shader.useProgram();
		 
	bounding_box_shader.set_uniform_location("model", mesh->camera.model);
	bounding_box_shader.set_uniform_location("view", mesh->camera.view);
	bounding_box_shader.set_uniform_location("projection", mesh->camera.projection);
		 
	bounding_box_shader.set_uniform_location("translate", mesh->translation_matrix);
	bounding_box_shader.set_uniform_location("scale", mesh->scale_matrix);
	bounding_box_shader.set_uniform_location("rotate", mesh->rotation_matrix);
		 
	bounding_box_shader.set_uniform_location("scalar", mesh->scale);

	glBindVertexArray(mesh->bounds_VAO);
	glDrawElements(GL_LINES, mesh->bounding_box_indices.size(), GL_UNSIGNED_INT, 0);
}

void Material::apply_ray_cast_shader(Mesh* mesh) {
	
	if (mesh->bounding_box_intersection_test()) {
		glm::vec3 ray_hit = mesh->get_ray_hit();

		ray_mesh->translation_matrix[3][0] = ray_hit[0];
		ray_mesh->translation_matrix[3][1] = ray_hit[1];
		ray_mesh->translation_matrix[3][2] = ray_hit[2];

		ray_cast_shader.useProgram();
		ray_cast_shader.set_uniform_location("model", mesh->camera.model);
		ray_cast_shader.set_uniform_location("view", mesh->camera.view);
		ray_cast_shader.set_uniform_location("projection", mesh->camera.projection);
		ray_cast_shader.set_uniform_location("translate", ray_mesh->translation_matrix);
		ray_cast_shader.set_uniform_location("scalar", .1f);

		glBindVertexArray(ray_mesh->VAO);
		glDrawElements(GL_TRIANGLES, ray_mesh->indices.size(), GL_UNSIGNED_INT, 0);
	}
}

void Material::spotlight_material_data(Spotlight_Mesh& spotlight) {
	ImGui::Begin(spotlight.name.c_str());
	spotlight.show_UI();
	ImGui::End();
}

void Material::attach_mesh(Light_Mesh& light_mesh) {
	mesh_objects.push_back(&light_mesh);
}
