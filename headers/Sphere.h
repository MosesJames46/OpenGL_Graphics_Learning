#pragma once
#include "libs.h"
#include "Shapes.h"
#include "Gui_Settings.h"
#include <sstream>

class Sphere : public Shape {
public:
	
	Sphere(const std::string& name, int stacks, int slices, int scale) : sphere_name(name)
	, stacks(stacks), slices(slices), radius(radius){
		//Good tip for transforming the this pointer into a string. Static case to a const void*.
		const void* a = static_cast<const void*>(this);
		std::stringstream ss;
		ss << a;
		std::string hash = "##";
		sphere_name = name + hash + ss.str();
	}

	void generate_mesh(Mesh& mesh) override;

	void set_radius(std::vector<float>& vertices);

	void generate_indices(std::vector<unsigned int>& indices) override;
	void generate_vertices(std::vector<float>& vertices) override;

	std::string sphere_name;

	/*
		Stacks for a sphere are the vertical squares that show how many squares are "stacked" vertically.
		Slices are the horizontal squares that span the spheres.
	*/
	int stacks, slices;
	float radius = 1;
};