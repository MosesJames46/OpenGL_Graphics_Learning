#include "../headers/Material.h"
#include "../headers/Shader.h"
#include "../headers/Mesh.h"
#include "../headers/Camera.h"

Material::Material(std::unique_ptr<Shader> shader, std::unique_ptr<Mesh> mesh, Camera* camera, material_type material) :
	material_data{ std::move(shader), std::move(mesh), camera }, material(material){
	info.mesh = material_data.mesh.get();
	info.shader = material_data.shader.get();
	info.camera = camera;
	//std::cout << info.mesh << std::endl;
}

Material::Material(std::unique_ptr<Shader> shader, std::unique_ptr<Mesh> mesh, material_type material):
	material_data{ std::move(shader), std::move(mesh), nullptr}, material(material) {
		info.mesh = material_data.mesh.get();
		info.shader = material_data.shader.get();
		info.camera = nullptr;
}

void Material::light_material(bool render) {
	info.shader->set_uniform_location("object_position", info.mesh->position);
	info.shader->set_uniform_location("object_color", info.mesh->color);

	if (render)
		light_material_data();
}

void Material::light_material_data() {
	ImGui::Begin(std::string(info.mesh->name).c_str());
	info.mesh->set_color();
	info.mesh->set_position();
	ImGui::End();
}

void Material::complex_material(Camera& camera, bool render) {
	
	info.shader->set_uniform_location("object_position", info.mesh->position);
	if (material == COMPLEX) {
		info.shader->set_uniform_location("material.ambient", info.mesh->ambience);
		info.shader->set_uniform_location("material.diffuse", info.mesh->color);
	}
	info.shader->set_uniform_location("material.specular", info.mesh->specular);
	info.shader->set_uniform_location("material.shininess", info.mesh->shininess);
	info.shader->set_uniform_location("view_position", camera.camera_origin);

	if (render)
		complex_material_data();
	if (mesh_objects.empty()) return;	
	for (auto& mesh_object : mesh_objects) {
		info.shader->set_uniform_location("light.ambient", mesh_object->ambience);
		info.shader->set_uniform_location("light.diffuse", mesh_object->color);
		info.shader->set_uniform_location("light.specular", mesh_object->specular);
		info.shader->set_uniform_location("light.position", mesh_object->position);
	}

	
}

void Material::complex_material_data() {
	ImGui::Begin(std::string(info.mesh->name).c_str());
	info.mesh->set_color();
	info.mesh->set_position();
	info.mesh->set_ambient();
	info.mesh->set_shininess();
	ImGui::End();
}

void Material::attach_mesh(Mesh& mesh) {
	mesh_objects.push_back(&mesh);
}