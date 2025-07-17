#include "../headers/Gui_Settings.h"
#include "../headers/Renderer.h"
#include "../headers/Mesh.h"
#include "../headers/Material.h"
#include "../headers/Shader.h"
#include "../headers/Camera.h"


struct Renderer_Data {
    std::string v;
    std::string f;
    std::string mesh_name;
    shape_type shape_ind;
    material_type material_ind;
    Camera& camera;
};

void Gui_Settings::call_new_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui_Settings::render_frame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui_Settings::gui_test(Camera& camera) {
    ImGui::Begin("Creation Window.");
    /*
        For all char arrays, they must be static in order to be modified as a single entity,
        over the time of the program.
    */
    
    
    // List of shaders that can be used. Sucks I must do every frame.
  
    /*
        Objects that must persist throughout the lifetime of the render must be made static.
    */
   

    /*
        Determine Shader Types
    */

    //The ++button_clicked method allows checking if the box should be rendered later
    static int button_clicked = 0;
    if (ImGui::Button("Add Window")) {
        ++button_clicked;
    }

    static bool create = false;

    if (button_clicked & 1) {
        ImGui::SeparatorText("Shader Selection");
        static int vertex_index = 0;
        std::string v = use_combo(vertex, "Vertex", vertex_index); ImGui::SameLine();

        static int fragment_index = 0;
        std::string f = use_combo(fragment, "Fragment", fragment_index);

        ImGui::SeparatorText("Mesh");
        static char mesh_name[64];
        ImGui::InputText("Mesh Name", mesh_name, 64); ImGui::SameLine();

        static int shape_index = 0;
        use_combo(shape, "Shape", shape_index);

        ImGui::SeparatorText("Material Type");
        static int material_index = 0;
        use_combo(material, "Material", material_index);

        
        std::string selected_file = process_textures();

        Renderer_Data render_data{ v, f, mesh_name, static_cast<shape_type>(shape_index),
        static_cast<material_type>(material_index), camera };

        if (ImGui::Button("Create")) {
            
            std::unique_ptr<Renderer> renderer = std::move(create_renderer(render_data, selected_file));
            renderers.emplace_back(std::move(renderer));
            renderer_names.emplace_back(renderers.back()->mesh.name);
        }
    }


   
    
    draw_meshes();
    ImGui::End();
}

void Gui_Settings::gui_test_type() {
    static int make = 0;

    ImGui::Begin("Testing");
    if (ImGui::Button("click")){
        ++make;
        windows.push_back(window());
    }

    for (auto i : windows) {
        i.rend();
    }
    ImGui::End();
   
}

std::unique_ptr<Renderer> Gui_Settings::create_renderer(Renderer_Data& render_data, const std::string& file) {
    switch (render_data.material_ind) {
    case LIGHT:
        return create_light(render_data);
    case COMPLEX:
        return create_complex(render_data);
    case TEXTURED:
        return create_textured(render_data, file);
    }
}


std::unique_ptr<Renderer> Gui_Settings::create_light(Renderer_Data& render_data) {
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(render_data.mesh_name, render_data.shape_ind);
    std::unique_ptr<Shader> shader = std::make_unique<Shader>(render_data.v.c_str(), render_data.f.c_str());
    std::unique_ptr<Material> material = std::make_unique<Material>(std::move(shader), std::move(mesh), &render_data.camera, render_data.material_ind);
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(std::move(material), material->info.camera);
    return renderer;
}

std::unique_ptr<Renderer> Gui_Settings::create_complex(Renderer_Data& render_data) {
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(render_data.mesh_name, render_data.shape_ind);;
    std::unique_ptr<Shader> shader = std::make_unique<Shader>(render_data.v.c_str(), render_data.f.c_str());
    std::unique_ptr<Material> material = std::make_unique<Material>(std::move(shader), std::move(mesh), &render_data.camera, render_data.material_ind);
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(std::move(material), material->info.camera);
    initialize_renderer(renderer.get());

    return renderer;
}

std::unique_ptr<Renderer> Gui_Settings::create_textured(Renderer_Data& render_data, const std::string& file){
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(render_data.mesh_name, render_data.shape_ind);
    std::unique_ptr<Shader> shader = std::make_unique<Shader>(render_data.v.c_str(), render_data.f.c_str());
    std::unique_ptr<Material> material = std::make_unique<Material>(std::move(shader), std::move(mesh), &render_data.camera, render_data.material_ind);
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(std::move(material), material->info.camera);
    
    //Apply textures selected in GUI
    renderer->add_textures({ file.c_str() }, { "material.diffuse" });
    initialize_renderer(renderer.get());
    return renderer;
}


std::string Gui_Settings::use_combo(std::vector<std::string>& arr, const char* name, int& ind) {
    std::string preview = arr[ind];
    if (ImGui::BeginCombo(name, preview.c_str(), ImGuiComboFlags_WidthFitPreview)) {
        for (int i = 0; i < arr.size(); ++i) {
            const bool is_shape = (ind == i);
            if (ImGui::Selectable(arr[i].c_str(), is_shape))
                ind = i;
            if (is_shape)
                ImGui::SetItemDefaultFocus();

            //Utility function to check that the Shiny option is selected
            Gui_Settings::complex = Gui_Settings::is_complex(arr[ind]);
        }
        ImGui::EndCombo();
    }

    return preview;
}

std::string Gui_Settings::use_combo(std::vector<std::filesystem::path>& arr, const char* name, int& ind) {
    std::string preview = arr[ind].string();
    if (ImGui::BeginCombo(name, preview.c_str(), ImGuiComboFlags_WidthFitPreview)) {
        for (int i = 0; i < arr.size(); ++i) {
            const bool is_shape = (ind == i);
            if (ImGui::Selectable(arr[i].string().c_str(), is_shape))
                ind = i;
            if (is_shape)
                ImGui::SetItemDefaultFocus();
        } 
        ImGui::EndCombo();
    }

    return preview;
}


void Gui_Settings::initialize_renderer(Renderer* renderer) {
    if (!renderers.empty()) {
        for (const auto& r : renderers) {
            if (r->material.material == LIGHT)
                renderer->material.attach_mesh(r->mesh);
        }
    }
}

void Gui_Settings::draw_meshes() {
    static std::string selected;
    if (!renderers.empty()) {
        for (auto& i : renderers) {
            //Every mesh attaches the most recent mesh to the end.
            //i->material.attach_mesh(renderers.back()->mesh);
            if (i->mesh.name == selected) {
                i->draw(true);
            }
            else {
                i->draw(false);
            }
        }

        static int obj = 0;
        selected = use_combo(renderer_names, "Objects", obj);
        //std::cout << selected << std::endl;
    }
}


std::vector<std::string> Gui_Settings::fragment = { "shaders/fragment_shader.glsl", 
 "shaders/sphere_fragment_shader.glsl","shaders/light_fragment_shader.glsl"};
std::vector<std::string> Gui_Settings::vertex = { "shaders/vertex_shader.glsl","shaders/sphere_vertex_shader.glsl" 
,"shaders/light_vertex_shader.glsl" };
std::vector<std::string> Gui_Settings::shape = { "Sphere" };
std::vector<std::string> Gui_Settings::material = { "Light", "Complex", "Textured"};
std::vector<std::string> Gui_Settings::renderer_names;
std::vector<std::filesystem::path> Gui_Settings::texture_file_paths;

std::vector<std::unique_ptr<Renderer>> Gui_Settings::renderers;
std::list<std::unique_ptr<Renderer>> Gui_Settings::renderer_list;

bool Gui_Settings::complex = false;

int window::i = 0;
std::vector<window> Gui_Settings::windows;