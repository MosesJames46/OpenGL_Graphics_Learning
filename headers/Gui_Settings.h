#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Gui_Settings {
public:
	static void call_new_frame();
	static void render_frame();
	//static void edit_object_size(Sphere& sphere, float& sphere_radius);
	static void gui_test();
	//static void apply_colors(Sphere& sphere, Shader& shader);
};