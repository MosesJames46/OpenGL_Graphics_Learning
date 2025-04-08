#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Gui_Settings {
public:
	static void call_new_frame();
	static void render_frame();
	static void gui_test();
};