#pragma once
#include <iostream>
#include "libs.h"
#include <vector>
#include "Shader.h"
#include <memory>

class Mesh;
class Renderer;
class Camera;

/*
	Spotlight is a combo type that has all elements of a camera with the inclusion of a cutoff.
	This is done for name simplicity. Having a type of spotlight felt easier than adding a cutoff to the camera class.
	Did not want to bloat the camera class for something so simple.
*/

class Light_Mesh;
class Complex_Mesh;
class Texture_Mesh;
class Spotlight_Mesh;

enum  material_type {
	LIGHT, COMPLEX, TEXTURED, DIRECTIONAL, SPOTLIGHT
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


/*
	EDITS TO BE DONE:
	Make the material data like camera, mesh, shaders etc simpler to use.
*/

class Material {
public:
	using light = void (Material::*)(Light_Mesh& light_mesh, bool render);
	using complex = void (Material::*)(Complex_Mesh& complex_mesh, bool render);
	using spotlight = void (Material::*)(Spotlight_Mesh& spotlight, bool renderer);

	Material(std::unique_ptr<Shader> shader, material_type material);
	Material(Shader* shader) : ray_shader(shader) {};

	~Material() {}
		
	/*
		Pointer to Member function operation that selects the function to load based on enum passed to constructor.
	*/
	light activate_material(Light_Mesh& light_mesh, bool render) {
		return &Material::light_material;
	}

	complex activate_material(Complex_Mesh& complex_mesh, bool render) {
		return &Material::complex_material;
	}

	spotlight activate_material(Spotlight_Mesh& spotlight, bool render) {
		return &Material::spotlight_material;
	}

	void attach_mesh(Light_Mesh& light_mesh);
	

	void apply_shader(Light_Mesh& light_mesh, bool render = true) {
		light_material(light_mesh, render);
	};
	void apply_shader(Complex_Mesh& complex_mesh, bool render = true) {
		complex_material(complex_mesh, render);
	};
	void apply_shader(Spotlight_Mesh& spotlight_mesh, bool render = true) {
		spotlight_material(spotlight_mesh, render);
	};



	material_type material;
	std::unique_ptr<Shader> shader;

	Shader stencil_shader{ "shaders/stencil_vs.glsl", "shaders/stencil_fs.glsl" };
	Shader bounding_box_shader{ "shaders/bounding_box_vs.glsl", "shaders/bounding_box_fs.glsl" };
	Shader ray_cast_shader{ "shaders/ray_shader_vs.glsl", "shaders/ray_shader_fs.glsl" };

	std::vector<Light_Mesh*> mesh_objects;

	bool is_textured = false;

	void apply_highlight_shader(Mesh* mesh);
	void apply_bounds_shader(Mesh* mesh);
	void apply_ray_cast_shader(Mesh* mesh);

	

private:
	void light_material(Light_Mesh& light_mesh, bool render = true);
	void light_material_data(Light_Mesh& light_mesh);

	void complex_material(Complex_Mesh& complex_mesh, bool render = true);
	void complex_material_data(Complex_Mesh& complex_mesh);

	void spotlight_material(Spotlight_Mesh& spotlight, bool render = true);
	void spotlight_material_data(Spotlight_Mesh& spotlight);

	void light_effects();
	void flashlight(Spotlight_Mesh* spotlight);

	Camera* camera_for_ray_cast;
	std::unique_ptr<Mesh> ray_mesh;
	Shader* ray_shader;
};
