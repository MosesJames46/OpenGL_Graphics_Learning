#pragma once
#include <iostream>
#include "libs.h"
#include <vector>
#include "../extern/imgui/imgui.h"
#include "../extern/imgui/backends/imgui_impl_glfw.h"
#include "../extern/imgui/backends/imgui_impl_opengl3.h"

class Shader;
class Mesh;
class Camera;

enum  material_type {
	LIGHT, SHINY
};

/*
	When doing pointer to memeber funcitons, it is easier to use typedef / using alias.

	The proper way to call a pointer to function memeber is to have :
		1. The object that calls the function to choose the right function to call.
		2. Dereferece that the returned address. 
		3. Use the object member accessor to call the dereferenced function.
		4. invoke with proper parameters.

	As a side note, remember that the invoke has operateor precedende, so stes 1 - 3 must be enclosed in parenthesis. 

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

class Material {
public:
	using light = void (Material::*)();
	using complex = void (Material::*)(Camera& camera);

	Material(Shader& shader, material_type material, Mesh& mesh) : info{ &shader, &mesh, nullptr } {}
	Material(Shader& shader, material_type material, Mesh& mesh, Camera& camera) : info{&shader, &mesh, &camera}, material(material){}
	

	/*
		Pointer to Member function operation that selects the function to load based on enum passed to constructor.
	*/
	light activate_material() {
		return &Material::light_material;
	}

	complex activate_material(Camera& camera) {
		return &Material::complex_material;
	}

	void attach_mesh(Mesh& material);

	material_type material;
	std::vector<Mesh*> mesh_objects;
	data info;

private:
	void light_material();
	void light_material_data();

	void complex_material(Camera& camera);
	void complex_material_data();
};
