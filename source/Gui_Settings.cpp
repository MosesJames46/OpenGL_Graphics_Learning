#include "../headers/Gui_Settings.h"
#include "../headers/Renderer.h"
#include "../headers/Mesh.h"
#include "../headers/Mesh_Types/Light_Mesh.h"
#include "../headers/Mesh_Types/Complex_Mesh.h"
#include "../headers/Mesh_Types/Texture_Mesh.h"
#include "../headers/Mesh_Types/Spotlight_Mesh.h"
#include "../headers/Flashlight.h"
#include "../headers/Material.h"
#include "../headers/Shader.h"
#include "../headers/Camera.h"

struct Renderer_Data {
    std::string v;
    std::string f;
    std::string mesh_name;
    shape_type shape_ind;
    material_type material_ind;
    std::string texture_file;
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
    process_textures();
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
        ImGui::SeparatorText("Mesh");
        static char mesh_name[64];
        ImGui::InputText("Mesh Name", mesh_name, 64); ImGui::SameLine();

        static int shape_index = 0;
        use_combo(shape, "Shape", shape_index);

        ImGui::SeparatorText("Material Type");
        static int material_index = 0;
        use_combo(material, "Material", material_index);

        static bool apply_texture;
        if (material_index != 0) {
            ImGui::Checkbox("Apply Texture", &apply_texture);
        }

        static std::string texture;
        static int texture_id = 0;
        if (apply_texture) {
            ImGui::SeparatorText("Textures");
            texture = select_texture_file("##Apply Texture", texture_id);
        }

        /*
            Small enough logic that a function is not needed.
            Select the propoer shaders based on material type.
        */

        //string values for vertex and fragment shaders
        std::string v;
        std::string f;

        /*
            Whenever we add a new shader this should be updated.
        */
        attach_shader(v, f, static_cast<material_type>(material_index));

        Renderer_Data render_data{ v, f, mesh_name, static_cast<shape_type>(shape_index),
        static_cast<material_type>(material_index), texture, camera };

        if (ImGui::Button("Create")) {
            
            std::unique_ptr<Renderer> renderer = std::move(create_renderer(render_data, apply_texture));
            renderers.emplace_back(std::move(renderer));
            renderer_names.emplace_back(renderers.back()->mesh->name);
        }
    }
    
    draw_meshes();
    get_world_position(camera);

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

std::unique_ptr<Renderer> Gui_Settings::create_renderer(Renderer_Data& render_data, bool is_textured) {
    switch (render_data.material_ind) {
    case LIGHT:
        return create_light(render_data);
    case COMPLEX:
        return create_complex(render_data, is_textured);
    case DIRECTIONAL:
        return create_directional(render_data, is_textured);
    case SPOTLIGHT:
        return create_spotlight(render_data, is_textured);
    }
}

std::unique_ptr<Renderer> Gui_Settings::create_light(Renderer_Data& render_data) {
    std::unique_ptr<Light_Mesh> light_mesh = std::make_unique<Light_Mesh>(render_data.mesh_name, render_data.shape_ind);
    std::unique_ptr<Shader> shader = std::make_unique<Shader>(render_data.v.c_str(), render_data.f.c_str());
    std::unique_ptr<Material> material = std::make_unique<Material>(std::move(shader), render_data.material_ind);
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(std::move(light_mesh), std::move(material), render_data.camera);
    return renderer;
}


/*
    Anytime there is a c2661 error within a header that states "no overloadded function takes n arguments", it most likely means
    there is an error with a unique pointer instantiation. 
*/
std::unique_ptr<Renderer> Gui_Settings::create_complex(Renderer_Data& render_data, bool is_textured) {
    std::unique_ptr<Complex_Mesh> complex_mesh = std::make_unique<Complex_Mesh>(render_data.camera, render_data.mesh_name, render_data.shape_ind);;
    std::unique_ptr<Shader> shader = std::make_unique<Shader>(render_data.v.c_str(), render_data.f.c_str());
    std::unique_ptr<Material> material = std::make_unique<Material>(std::move(shader), render_data.material_ind);
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(std::move(complex_mesh), std::move(material), render_data.camera);

    if (is_textured) {
        renderer->add_textures(*renderer->material->shader.get(), { render_data.texture_file.c_str() }, { "material.diffuse" });
    }

    initialize_renderer(renderer.get());

    return renderer;
}

//Textured is a complex model that uses textures to improves that look of an object.
std::unique_ptr<Renderer> Gui_Settings::create_textured(Renderer_Data& render_data, const std::string& file){
    std::unique_ptr<Mesh> texture_mesh = std::make_unique<Mesh>(render_data.mesh_name, render_data.shape_ind);
    std::unique_ptr<Shader> shader = std::make_unique<Shader>(render_data.v.c_str(), render_data.f.c_str());
    std::unique_ptr<Material> material = std::make_unique<Material>(std::move(shader), render_data.material_ind);
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(std::move(texture_mesh), std::move(material), render_data.camera);
    
    //Apply textures selected in GUI
    
    renderer->add_textures(*renderer->material->shader.get() , {file.c_str()}, {"material.diffuse"});
    initialize_renderer(renderer.get());
    return renderer;
}

//Directional is a type of lighting calculation that only uses direction.
std::unique_ptr<Renderer> Gui_Settings::create_directional(Renderer_Data& render_data, bool is_bool) {
    return create_complex(render_data, is_bool);
}

std::unique_ptr<Renderer> Gui_Settings::create_spotlight(Renderer_Data& render_data, bool is_textured) {
    std::unique_ptr<Spotlight_Mesh> spotlight_mesh = std::make_unique<Spotlight_Mesh>(render_data.camera, render_data.mesh_name, render_data.shape_ind, is_textured);
    std::unique_ptr<Shader> shader = std::make_unique<Shader>(render_data.v.c_str(), render_data.f.c_str());
    std::unique_ptr<Material> material = std::make_unique<Material>(std::move(shader), render_data.material_ind);
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(std::move(spotlight_mesh), std::move(material), render_data.camera);

    if (is_textured) {
        renderer->add_textures(*renderer->material->shader.get(), { render_data.texture_file.c_str() }, { "material.diffuse" });
    }
    return renderer;
}

void Gui_Settings::attach_shader(std::string& vertex_shader, std::string& fragment_shader, material_type material_index) {
    switch (static_cast<material_type>(material_index)) {
    case LIGHT:
        vertex_shader = vertex[0];
        fragment_shader = fragment[0];
        break;
    case COMPLEX:
        vertex_shader = vertex[1];
        fragment_shader = fragment[1];
        break;
    case TEXTURED:
        vertex_shader = vertex[2];
        fragment_shader = fragment[2];
        break;
    case DIRECTIONAL:
        vertex_shader = vertex[3];
        fragment_shader = fragment[3];
        break;
    case SPOTLIGHT:
        vertex_shader = vertex[4];
        fragment_shader = fragment[4];
        break;
    }
}

/*
    Generalization function for combo boxes. Just input your vector of strings and the function will make a dropdown box.
*/
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

std::string Gui_Settings::select_texture_file(const char* name, int& ind) {
    return use_combo(texture_file_paths, name, ind);
}

void Gui_Settings::process_textures() {
    static int complex_ind = 0;
    static std::string directory_path = "./pictures/";
    if (texture_file_paths.empty())
        for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
            texture_file_paths.push_back(entry.path().string());
        }
}

void Gui_Settings::initialize_renderer(Renderer* renderer) {
    if (!renderers.empty()) {
        for (const auto& r : renderers) {
            switch (r->material->material) {
            case LIGHT:
                renderer->material->attach_mesh(dynamic_cast<Light_Mesh&>(*r->mesh.get()));
                break;
            case SPOTLIGHT:
                renderer->material->attach_mesh(dynamic_cast<Spotlight_Mesh&>(*r->mesh.get()));
                break;
            }
        }
    }
}

void Gui_Settings::draw_meshes() {
    static std::string selected;
    if (!renderers.empty()) {
        for (auto& i : renderers) {
            //Every mesh attaches the most recent mesh to the end.
            //i->material.attach_mesh(renderers.back()->mesh);
            if (i->mesh->name == selected) {
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

void Gui_Settings::get_world_position(Camera& camera) {
    static int f2_press = 0;
    if (ImGui::IsKeyPressed(ImGuiKey_F2))++f2_press;
    if (f2_press & 1) {
        ImGui::Begin("Camera Info");
        ImGui::Text("Camera Positions: (%g, %g, %g)", camera.camera_origin.x, camera.camera_origin.y, camera.camera_origin.z);
        ImGui::Text("Camera Forward: (%g, %g, %g)", camera.camera_forward.x, camera.camera_forward.y, camera.camera_forward.z);
        ImGui::End();
    }
    
}


std::vector<std::string> Gui_Settings::fragment = { "shaders/light_fs.glsl", 
 "shaders/complex_fs.glsl","shaders/textured_fs.glsl", "shaders/direction_light_fs.glsl",
    "shaders/spotlight_fs.glsl" };
std::vector<std::string> Gui_Settings::vertex = { "shaders/light_vs.glsl","shaders/complex_vs.glsl" 
,"shaders/textured_vs.glsl", "shaders/direction_light_vs.glsl","shaders/spotlight_vs.glsl" };
std::vector<std::string> Gui_Settings::shape = { "Sphere"};
std::vector<std::string> Gui_Settings::material = { "Light", "Complex", "Textured", "Directional", "Spotlight"};
std::vector<std::string> Gui_Settings::renderer_names;
std::vector<std::string> Gui_Settings::texture_file_paths;

std::vector<std::unique_ptr<Renderer>> Gui_Settings::renderers;
std::list<std::unique_ptr<Renderer>> Gui_Settings::renderer_list;

bool Gui_Settings::complex = false;

int window::i = 0;
std::vector<window> Gui_Settings::windows;