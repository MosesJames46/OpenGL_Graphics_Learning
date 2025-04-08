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

void Gui_Settings::gui_test() {
    ImGui::Begin("Testing Window");
    
    ImGui::End();
}
