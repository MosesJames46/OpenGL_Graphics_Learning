#include "../headers/Gui_Settings.h"
void Gui_Settings::call_new_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui_Settings::render_frame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//void Gui_Settings::edit_object_size(Sphere& sphere, float& sphere_radius) {
//    ImGui::Begin("Sphere");
//    ImGui::SliderFloat("float", &sphere_radius, 0.1f, 10.0f);
//    sphere.set_radius(sphere_radius);
//    ImGui::End();
//}

void Gui_Settings::gui_test() {
    //static float color = 0;
    //static glm::vec3 color{ 0.0f, 0.0f, 0.0f };
    ImGui::Begin("Testing Window");
    
    ImGui::End();
}
