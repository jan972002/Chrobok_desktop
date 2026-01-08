#include "GuiModule.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "ConfigManager.h"
#include "Icons.h"
#include "Logic.h"

float GuiModule::menuBarHeight = 0.0f;
void GuiModule::Setup(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    static const ImWchar ranges[] = { 0x0020, 0x00FF, 0x0100, 0x017F, 0 };
    io.Fonts->AddFontFromFileTTF("fonts/tahoma.ttf", 20.0f, nullptr, ranges);

    static const ImWchar icons_ranges[] = { 0xf000, 0xf8ff, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("fonts/icons_solid.otf", 20.0f, &icons_config, icons_ranges);



    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGui::GetStyle().WindowRounding = 8.0f;
    ImGui::GetStyle().FrameRounding = 5.0f;
    ImGuiStyle& style = ImGui::GetStyle();
    // Style okien
    style.WindowRounding = 7.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 12.0f;
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(8, 6);
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]          = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_Header]            = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    colors[ImGuiCol_HeaderHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button]            = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ButtonHovered]     = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ButtonActive]      = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_FrameBg]           = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]    = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgActive]     = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TitleBg]           = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]     = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_CheckMark]         = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]        = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]  = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
}
// Granice
void GuiModule::WymusGraniceOkna(const char* name) {
    ImGuiWindow* window = ImGui::FindWindowByName(name);
    if (window) {
        ImVec2 pos = window->Pos;
        ImVec2 size = window->Size;
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;

        ImVec2 nowaPos = pos;
        bool koryguj = false;
        if (nowaPos.x < 0) { nowaPos.x = 0; koryguj = true; }
        if (nowaPos.y < menuBarHeight) { nowaPos.y = menuBarHeight; koryguj = true; }
        if (nowaPos.x + size.x > displaySize.x) { nowaPos.x = displaySize.x - size.x; koryguj = true; }
        if (nowaPos.y + size.y > displaySize.y) { nowaPos.y = displaySize.y - size.y; koryguj = true; }
        if (koryguj) {
            ImGui::SetNextWindowPos(nowaPos, ImGuiCond_Always);
        }
    }
}

void GuiModule::RenderFrame(GLFWwindow* window, AppState& state, int monitorCount, GLFWmonitor** monitors) {
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = state.skala_tekstu;
    ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(state.textColor[0], state.textColor[1], state.textColor[2], 1.0f);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Menu
    if (ImGui::BeginMainMenuBar()) {
        menuBarHeight = ImGui::GetWindowSize().y;
        if (ImGui::BeginMenu("Plik")) {
            if (ImGui::MenuItem(ICON_FA_SAVE " Zapisz", "Ctrl+S")) ConfigManager::Zapisz(state);
            if (ImGui::MenuItem(ICON_FA_LOAD " Wczytaj", "Ctrl+L")) ConfigManager::Laduj(state);
            ImGui::Separator();
            if (ImGui::MenuItem("Wyjście", "Esc")) glfwSetWindowShouldClose(window, true);
            ImGui::EndMenu();
        }
        ImGui::Spacing;
        if (ImGui::BeginMenu("Okna")) {
            ImGui::MenuItem("Sterowanie Ramienia", nullptr, &state.show_sterowanie_ramienia);
            ImGui::MenuItem("Parametry Ramienia", nullptr, &state.show_parametry_ramienia);
            ImGui::MenuItem("Sterowanie Ruchem", nullptr, &state.show_sterowanie_ruchem);
            ImGui::MenuItem("Detekcja Obrazu", nullptr, &state.show_detekcja_obrazu);
            ImGui::MenuItem("Odczyt Lidar", nullptr, &state.show_odczyt_lidar);
            ImGui::Separator();
            ImGui::MenuItem("Ustawienia", nullptr, &state.show_ustawienia_aplikacji);
            ImGui::EndMenu();
        }
        ImGui::Spacing;
        if (ImGui::BeginMenu("Informacje")) {
            ImGui::MenuItem("O Aplikacji", nullptr, &state.show_aboutApp_info);
            ImGui::MenuItem("O Projekcie", nullptr, &state.show_aboutProject_info);
            ImGui::MenuItem("O Nas", nullptr, &state.show_aboutUs_info);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Okna
    if (state.show_sterowanie_ramienia) {
        WymusGraniceOkna("Sterowanie Ramienia");
        ImGui::Begin("Sterowanie Ramienia", &state.show_sterowanie_ramienia, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SliderInt(" M_1 ", &state.M_1, 0, 2380);
            ImGui::SliderInt(" M_2 ", &state.M_2, 0, 150);
            ImGui::SliderInt(" M_3 ", &state.M_3, 0, 180);
            ImGui::SliderInt(" M_4 ", &state.M_4, 0, 180);
            ImGui::SliderInt(" M_5 ", &state.M_5, 0, 180);
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            if (ImGui::Button("Zerowanie Ramienia")) {Logic::ZerowanieRamienia(state); }
        ImGui::End();
    }
    if (state.show_aboutApp_info) {
        WymusGraniceOkna("O Aplikacji");
        ImGui::Begin("O Aplikacji", &state.show_aboutApp_info, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::Text("Nazwa: %s", state.nazwaAplikacji.c_str());
            ImGui::Text("Data Wydania: %s", state.dataWydania.c_str());
            ImGui::Text("Wersja: %s", state.wersjaAplikacji.c_str());
        ImGui::End();
    }
    if (state.show_aboutUs_info) {
        WymusGraniceOkna("O Nas");
        ImGui::Begin("O Nas", &state.show_aboutUs_info, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::Dummy(ImVec2(200, 0));
            ImGui::TextWrapped("%s" ,state.oNas.c_str());
        ImGui::End();
    }
    if (state.show_aboutProject_info) {
        WymusGraniceOkna("O Projekcie");
        ImGui::Begin("O Projekcie", &state.show_aboutProject_info,  ImGuiWindowFlags_NoCollapse);
            ImGui::Dummy(ImVec2(200, 0));
            ImGui::TextWrapped("%s" ,state.oProjekcie.c_str());
        ImGui::End();
    }
    if (state.show_parametry_ramienia) {
        WymusGraniceOkna("Parametry ramienia");
        ImGui::Begin("Parametry ramienia", &state.show_parametry_ramienia, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SliderInt(" Opoznienie chwytaka ", &state.opoznienieChwytaka, 0, 5);
            ImGui::SliderInt(" Opoznienie Serwa ", &state.opoznienieServa, 0, 20);
            ImGui::SliderInt(" Sila skretu ", &state.silaSkretu, 0, 100);
            ImGui::SliderInt(" Wartosc PWM ", &state.PWM, 0, 255);
            ImGui::Separator();
        ImGui::End();
    }

    if (state.show_sterowanie_ruchem) {
        WymusGraniceOkna("Ruch");
        ImGui::Begin("Ruch", &state.show_sterowanie_ruchem, ImGuiWindowFlags_AlwaysAutoResize );
            int stale_x = 100;int stale_y = 60;
            ImGui::Dummy(ImVec2(stale_x, stale_y));ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_UP "##Przod", ImVec2(stale_x, stale_y)) ) {}
            if (ImGui::Button(ICON_FA_ARROW_LEFT "##Lewo", ImVec2(stale_x, stale_y))) {} ImGui::SameLine();
            if (ImGui::Button(ICON_FA_STOP "##Stop", ImVec2(stale_x, stale_y))) {} ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_RIGHT "##Prawo", ImVec2(stale_x, stale_y))) {}
            ImGui::Dummy(ImVec2(stale_x, stale_y));ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_DOWN "##Tyl", ImVec2(stale_x, stale_y))) {}
            ImGui::SameLine();ImGui::Dummy(ImVec2(stale_x, stale_y));
            if (ImGui::Button(ICON_FA_ROTATE_LEFT "##OBR.L", ImVec2(stale_x, stale_y))) {}
            ImGui::SameLine();ImGui::Dummy(ImVec2(stale_x,stale_y));ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ROTATE_RIGHT "##OBR.P", ImVec2(stale_x, stale_y))) {}

        ImGui::End();
    }

    // Ustawienia
    if (state.show_ustawienia_aplikacji) {
        WymusGraniceOkna("Ustawienia");
        ImGui::Begin("Ustawienia", &state.show_ustawienia_aplikacji, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text(" Wybierz monitor ");
            const char* preview = glfwGetMonitorName(monitors[state.selected_monitor]);
            if (ImGui::BeginCombo("##Wybierz Monitor", preview)) {
                for (int n = 0; n < monitorCount; n++) {
                    if (ImGui::Selectable(glfwGetMonitorName(monitors[n]), state.selected_monitor == n)) {
                        state.selected_monitor = n;
                        const GLFWvidmode* m = glfwGetVideoMode(monitors[n]);
                        int x, y; glfwGetMonitorPos(monitors[n], &x, &y);
                        glfwSetWindowMonitor(window, NULL, x, y, m->width, m->height, m->refreshRate);
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::Separator();
            ImGui::Text(" Skala Interfejsu ");
            if (ImGui::Button("-", ImVec2(40, 40))) { if (state.skala_tekstu > 0.5f) state.skala_tekstu -= 0.1f; }
            ImGui::SameLine(); ImGui::Text(" %.1f ", state.skala_tekstu);
            ImGui::SameLine(); if (ImGui::Button("+", ImVec2(40, 40))) { if (state.skala_tekstu < 3.5f) state.skala_tekstu += 0.1f; }

            ImGui::ColorEdit3(" Kolor tła", state.bgColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit3(" Kolor czcionki", state.textColor, ImGuiColorEditFlags_NoInputs);
            ImGui::Checkbox(" Pokaz statystyki FPS", &state.show_debug_info);
        ImGui::End();
    }


    if (state.show_detekcja_obrazu) {
        WymusGraniceOkna("Detekcja Obrazu");
        ImGui::Begin("Detekcja Obrazu", &state.show_detekcja_obrazu, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Tu wyląduje tekstura z OpenCV...");
            ImGui::Dummy(ImVec2(400, 300));
        ImGui::End();
    }
    if (state.show_odczyt_lidar) {
        WymusGraniceOkna("Odczyt Lidar");
        ImGui::Begin("Odczyt Lidar",&state.show_odczyt_lidar, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Tu bedzie wykres lidaru + odleglosc?");
            ImGui::Dummy(ImVec2(400, 300));
        ImGui::End();
    }
    // FPS
    if (state.show_debug_info) {
        ImGui::SetNextWindowPos(ImVec2(10, io.DisplaySize.y - 10), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
        ImGui::Begin("FPS_Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Wydajnosc: %.3f ms (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    ImGui::Render();
}

void GuiModule::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}