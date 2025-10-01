#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include <list>
#include <memory>
#include <string>
#include <filesystem>



/*
	[COMBO BOXES]
	---------------------------
	/ All Combo Boxes will have the beginning if(BeginCombo())//
	/ Always ends with EndCombo()							  //
	/ The general idea is to always have come static value that starts at the 0
		index. This value will then be the checked item in the range of a given array.
		We use IM_ARRAYSIZE to make ease of the process of our array. It is just the 
		standard way of checking an array size in a for - loop.//
	 / const bool is_selected = (item_selected_idx == n);
            if (ImGui::Selectable(items[n], is_selected))
                item_selected_idx = n;
		- Check if the valuse is selected. 
		- item_selected_idx is the static int and n is the for loop index.
		- the ImGui::Selectable ensures that value can be hoverable and change colors when appropriate.
*/


/*
	[BUTTONS STAY OVER TIME]

	-The following is an example of keeping a button that's clicked on over time.

	static int clicked = 0;
		if (ImGui::Button("Button"))
			clicked++;
		if (clicked & 1)
		{
			ImGui::SameLine();
			ImGui::Text("Thanks for clicking me!");
		}
*/
class Shader;
class Camera;
class Mesh;
class Material;
class Renderer;

class Spotlight;

enum material_type;

/*
	Holds information needed to determine the object to create.
*/
struct Renderer_Data;

class window {
public:
	static int i;
	std::string name = std::to_string(i++);
	void rend() {
		ImGui::Begin(name.c_str());
		ImGui::End();
	}
};


class Gui_Settings {
public:

	~Gui_Settings() {}
	static void call_new_frame();
	static void render_frame();
	static void gui_test(Camera& camera);
	static void gui_test_type();

	
	/*
		Should create a renderer based on the material type requested.

		- Reuqiures:
			1. Mesh / Mesh Name
			2. Material objext
			3. Shader information
		Can pass the material for all info.
	*/


private:
	inline static bool is_complex(std::string string) {
		return string == "Textured";
	}


	static std::unique_ptr<Renderer> create_renderer(Renderer_Data& render_data, bool is_textured);

	static std::unique_ptr<Renderer> create_light(Renderer_Data& render_data);
	static std::unique_ptr<Renderer> create_complex(Renderer_Data& render_data, bool is_complex);
	static std::unique_ptr<Renderer> create_textured(Renderer_Data& render_data, const std::string& file);
	static std::unique_ptr<Renderer> create_directional(Renderer_Data& render_data, bool is_textured);
	static std::unique_ptr<Renderer> create_spotlight(Renderer_Data& render_data, bool is_complex);


	/*Function utility to make selecting proper shaders easier*/
	static void attach_shader(std::string& vertex_shader, std::string& fragment_shader, material_type material_index);


	/*
		The use combo function generalizes combo boxes to one function for ease.

		For reference see line 1270 in the ImGui_demo.cpp file.
	*/
	static std::string use_combo(std::vector<std::string>& v, const char* name, int& idx);
	static std::string select_texture_file(const char* name, int& idx);

	//Function to take a new renderer and attach current renderer meshes
	static void initialize_renderer(Renderer* renderer);
	static void get_world_position(Camera& camera);
	/*
		TODO: Implement deleting of a Renderer.
		TODO: Add dynamic texture application.
	*/
	static void draw_meshes();
	/*
		Uses the pictures directory to show useable textures to apply to a mesh.
	*/
	static void process_textures();
	static void input_text();

	static std::vector<std::unique_ptr<Renderer>> renderers;
	static std::list<std::unique_ptr<Renderer>> renderer_list;
	
	static bool complex;

	static std::vector<std::string> fragment;
	static std::vector<std::string> vertex;
	static std::vector<std::string> shape;
	static std::vector<std::string> material;
	static std::vector<std::string> renderer_names;

	static std::vector<std::string> texture_file_paths;
	
	static std::vector<window> windows;
};