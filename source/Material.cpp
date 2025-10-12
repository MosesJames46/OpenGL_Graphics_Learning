#include "../headers/Material.h"
#include "../headers/Shader.h"
#include "../headers/Mesh.h"
#include "../headers/Camera.h"
#include "../headers/Mesh_Types/Spotlight_Mesh.h"
#include "../headers/Mesh_Types/Light_Mesh.h"
#include "../headers/Mesh_Types/Complex_Mesh.h"
#include "../headers/Mesh_Types/Texture_Mesh.h"

Material::Material(std::unique_ptr<Shader> shader, material_type material) :
	shader(std::move(shader)), material(material), is_textured(is_textured) {}


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

Shader Material::apply_highlight_shader(Mesh* mesh) {
	//After the draw call, use the glStnecilFunc to check if any values are not equal.
	//Depth testing is disabled to that our newly rendered object is behind our current object.
	//The stencil mask is set to zero which basically stops the stencil mask from updating.
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	Shader highlight_shader("shaders/basic_vs.glsl", "shaders/basic_fs.glsl");
	highlight_shader.useProgram();
	highlight_shader.set_uniform_location("model", mesh->camera.model);
	highlight_shader.set_uniform_location("view", mesh->camera.view);
	highlight_shader.set_uniform_location("projection", mesh->camera.projection);
	highlight_shader.set_uniform_location("translation", mesh->translation_matrix);
	highlight_shader.set_uniform_location("rotation", mesh->rotation_matrix);
	highlight_shader.set_uniform_location("scale", mesh->scale_matrix);
	highlight_shader.set_uniform_location("object_position", mesh->position);
	highlight_shader.set_uniform_location("scalar", mesh->scale);

	//Render the triangle but with the newly attached shader. 
	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

	//At this point we want to turn the stencil buffer back on to overwrite the values that are zero.
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
	return highlight_shader;
}

void Material::spotlight_material_data(Spotlight_Mesh& spotlight) {
	ImGui::Begin(spotlight.name.c_str());
	spotlight.show_UI();
	ImGui::End();
}

void Material::attach_mesh(Light_Mesh& light_mesh) {
	mesh_objects.push_back(&light_mesh);
}