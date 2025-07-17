#pragma once
#include <iostream>
#include "libs.h"
#include <vector>
#include "../extern/imgui/imgui.h"
#include "../extern/imgui/backends/imgui_impl_glfw.h"
#include "../extern/imgui/backends/imgui_impl_opengl3.h"
#include <memory>

class Shader;
class Mesh;
class Camera;

enum  material_type {
	LIGHT, COMPLEX, TEXTURED
};

/*
	When doing pointer to memeber funcitons, it is easier to use typedef / using alias.

	The proper way to call a pointer to function memeber is to have :
		1. The object that calls the function to choose the right function to call.
		2. Dereferece that the returned address. 
		3. Use the object member accessor to call the dereferenced function.
		4. invoke with proper parameters.

	As a side note, remember that the invoke has operateor precedende, so steps 1 - 3 must be enclosed in parenthesis. 

	Example: 
		T object;
		Ptr_To_Mem*  ptm = object.get_pointer_to_member();
		(object.*ptm)(parameter list)

	Further analysis shows that any form of pointer to memeber function access must be done with the class scope.
	
	To store the address of a pointer to memeber functoin we must use the address of the class.
		- &Class::function
		
*/

struct data {
	Shader* shader;
	Mesh* mesh;
	Camera* camera;
};

struct material_data_unique {
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Mesh> mesh;
	Camera* camera;
};


class Material {
public:
	using light = void (Material::*)(bool render);
	using complex = void (Material::*)(Camera& camera, bool render);

	Material(Shader& shader, Mesh& mesh, material_type mate5rial) : info{ &shader, &mesh, nullptr },
		material_data{ nullptr, nullptr, nullptr }, material(material) {
	}
	Material(Shader& shader, Mesh& mesh, Camera& camera, material_type material) : info{&shader, &mesh, &camera}, material(material){}
	Material(Shader* shader, Mesh* mesh, Camera* camera, material_type material) : info{ shader, mesh, camera }, material(material) {}
	Material(std::unique_ptr<Shader> shader, std::unique_ptr<Mesh> mesh, Camera* camera, material_type material);
	Material(std::unique_ptr<Shader> shader, std::unique_ptr<Mesh> mesh, material_type material);
	~Material() {}
		
	/*
		Pointer to Member function operation that selects the function to load based on enum passed to constructor.
	*/
	light activate_material(bool render) {
		return &Material::light_material;
	}

	complex activate_material(Camera& camera, bool render) {
		return &Material::complex_material;
	}

	void attach_mesh(Mesh& material);

	material_type material;
	std::vector<Mesh*> mesh_objects;
	data info;
	

private:
	material_data_unique material_data;
	void light_material(bool render = true);
	void light_material_data();

	void complex_material(Camera& camera, bool render = true);
	void complex_material_data();
};
