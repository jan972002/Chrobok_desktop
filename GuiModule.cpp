#include "GuiModule.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include <fstream>
#include <vector>
#include <string>
#include "ConfigManager.h"
#include "Icons.h"
#include "Logic.h"

float GuiModule::menuBarHeight = 0.0f;
void GuiModule::Setup(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Spróbuj załadować przyjazną czcionkę systemową na różnych platformach, uwu~
    // Jeśli nic nie znajdziemy, użyjemy domyślnej czcionki ImGui, owo ~nya
    std::vector<std::string> candidates;
#if defined(_WIN32)
    candidates = { "C:/Windows/Fonts/arial.ttf", "C:/Windows/Fonts/segoeui.ttf", "C:/Windows/Fonts/Tahoma.ttf" };
#elif defined(__APPLE__)
    candidates = { "/Library/Fonts/Arial.ttf", "/System/Library/Fonts/Helvetica.ttc", "/System/Library/Fonts/Supplemental/Arial.ttf" };
#else
    candidates = { "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", "/usr/share/fonts/truetype/freefont/FreeSans.ttf" };
#endif

    // Użyj zakresu glifów Latin + Latin Extended-A, żeby obsłużyć polskie znaki diakrytyczne (ąćęłńóśźż), uwu
    static const ImWchar glyphRanges[] = { 0x0020, 0x00FF, 0x0100, 0x017F, 0 };
    bool loaded = false;
    for (const auto& path : candidates) {
        std::ifstream f(path);
        if (!f.good()) continue;
        f.close();
        if (io.Fonts->AddFontFromFileTTF(path.c_str(), 16.0f, nullptr, glyphRanges)) {
            loaded = true;
            break;
        }
    }
    if (!loaded) {
                // Nie znaleziono żadnej czcionki systemowej, więc fallbackujemy na domyślną czcionkę ImGui, so kawaii owo
                io.Fonts->AddFontDefault();
    }



    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Ustawienia stylu okien, bo muszą być cute i zaokrąglone, uwu
    ImGui::GetStyle().WindowRounding = 8.0f;
    ImGui::GetStyle().FrameRounding = 5.0f;
    ImGuiStyle& style = ImGui::GetStyle();
    // Styl okien, słodko i przytulnie owo
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
    // Granice okienka — ściskamy po ładnemu z pomocą ternary, nya~
void GuiModule::WymusGraniceOkna(const char* name) {
    const ImGuiWindow* window = ImGui::FindWindowByName(name);
    if (!window) return;

    const ImVec2 pos = window->Pos;
    const ImVec2 size = window->Size;
    const ImVec2 displaySize = ImGui::GetIO().DisplaySize;

    // Lepsze niż milion ifów: pojedyncze ternary ogarniają granice za jednym zamachem, uwu
    const ImVec2 clampedPos = {
        (pos.x < 0) ? 0.0f : (pos.x + size.x > displaySize.x) ? displaySize.x - size.x : pos.x,
        (pos.y < menuBarHeight) ? menuBarHeight : (pos.y + size.y > displaySize.y) ? displaySize.y - size.y : pos.y
    };

    if (clampedPos.x != pos.x || clampedPos.y != pos.y) {
        ImGui::SetNextWindowPos(clampedPos, ImGuiCond_Always);
    }
}

void GuiModule::RenderFrame(GLFWwindow* window, AppState& state, int monitorCount, GLFWmonitor** monitors) {
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = state.skala_tekstu;
    ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(state.textColor[0], state.textColor[1], state.textColor[2], 1.0f);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Menu główne (tu są te wszystkie przyciski, bardzo adorable owo)
    if (ImGui::BeginMainMenuBar()) {
        menuBarHeight = ImGui::GetWindowSize().y;
        if (ImGui::BeginMenu("Plik")) {
            if (ImGui::MenuItem(ICON_FA_SAVE " Zapisz", "Ctrl+S")) ConfigManager::Zapisz(state);
            if (ImGui::MenuItem(ICON_FA_LOAD " Wczytaj", "Ctrl+L")) ConfigManager::Laduj(state);
            ImGui::Separator();
            if (ImGui::MenuItem("Wyjście", "Esc")) glfwSetWindowShouldClose(window, true);
            ImGui::EndMenu();
        }
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
        if (ImGui::BeginMenu("Informacje")) {
            ImGui::MenuItem("O Aplikacji", nullptr, &state.show_aboutApp_info);
            ImGui::MenuItem("O Projekcie", nullptr, &state.show_aboutProject_info);
            ImGui::MenuItem("O Nas", nullptr, &state.show_aboutUs_info);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Renderowanie okienek — podejście data-driven, bo jestem leniwy i lubię tidy code, uwu
    // Łatwiej dodawać/usuwac okienka niż w gigantycznym łańcuchu if-else, nya

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
        if (ImGui::Button("Zerowanie Ramienia")) { Logic::ZerowanieRamienia(state); }
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
        ImGui::TextWrapped("%s", state.oNas.c_str());
        ImGui::End();
    }

    if (state.show_aboutProject_info) {
        WymusGraniceOkna("O Projekcie");
        ImGui::Begin("O Projekcie", &state.show_aboutProject_info, ImGuiWindowFlags_NoCollapse);
        ImGui::Dummy(ImVec2(200, 0));
        ImGui::TextWrapped("%s", state.oProjekcie.c_str());
        ImGui::End();
    }

    if (state.show_sterowanie_ruchem) {
        WymusGraniceOkna("Ruch");
        ImGui::Begin("Ruch", &state.show_sterowanie_ruchem, ImGuiWindowFlags_AlwaysAutoResize);
        constexpr int btnW = 100, btnH = 60;
        constexpr ImVec2 btnSize(btnW, btnH);

        // Układ przycisków oparty na danych — mniej powtarzalnego kodu, more cute owo
        // Prościej dodawać tryby ruchu i ogólnie rządzić przyciskami, uwu
        struct MovementButton {
            const char* label;
            bool* state;
        };

        // Row 1: Forward
        ImGui::Dummy(ImVec2(btnW, btnH)); ImGui::SameLine();
        ImGui::Button(ICON_FA_ARROW_UP "##Przod", btnSize);
        state.ruch_przod = ImGui::IsItemActive();

        // Row 2: Left, Stop, Right
        ImGui::Button(ICON_FA_ARROW_LEFT "##Lewo", btnSize);
        state.ruch_lewo = ImGui::IsItemActive();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STOP "##Stop", btnSize)) {
            state.zatrzymanie = true;
        }
        ImGui::SameLine();
        ImGui::Button(ICON_FA_ARROW_RIGHT "##Prawo", btnSize);
        state.ruch_prawo = ImGui::IsItemActive();

        // Row 3: Backward
        ImGui::Dummy(ImVec2(btnW, btnH)); ImGui::SameLine();
        ImGui::Button(ICON_FA_ARROW_DOWN "##Tyl", btnSize);
        state.ruch_tyl = ImGui::IsItemActive();

        // Kontrolki obrotu, bo robot też chce zakręcić główką uwu
        ImGui::Spacing();
        ImGui::Button(ICON_FA_ROTATE_LEFT "##OBR.L", btnSize);
        state.skret_lewo = ImGui::IsItemActive();
        ImGui::SameLine(); ImGui::Dummy(ImVec2(btnW, btnH)); ImGui::SameLine();
        ImGui::Button(ICON_FA_ROTATE_RIGHT "##OBR.P", btnSize);
        state.skret_prawo = ImGui::IsItemActive();
        ImGui::End();
    }

    // Ustawienia aplikacji — tu skala, kolory i inne bajery, owo
    if (state.show_ustawienia_aplikacji) {
        WymusGraniceOkna("Ustawienia");
        ImGui::Begin("Ustawienia", &state.show_ustawienia_aplikacji, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text(" Wybierz monitor ");
        if (const char* preview = glfwGetMonitorName(monitors[state.selected_monitor]); ImGui::BeginCombo("##Wybierz Monitor", preview)) {
            for (int n = 0; n < monitorCount; n++) {
                if (ImGui::Selectable(glfwGetMonitorName(monitors[n]), state.selected_monitor == n)) {
                    state.selected_monitor = n;
                    const GLFWvidmode* m = glfwGetVideoMode(monitors[n]);
                    int x, y; glfwGetMonitorPos(monitors[n], &x, &y);
                    glfwSetWindowMonitor(window, nullptr, x, y, m->width, m->height, m->refreshRate);
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Separator();
        ImGui::Text(" Skala Interfejsu ");
        // Lepiej niż osobne ify: ograniczamy wartość inline z ternary, taki sprytny sposób, uwu
        if (ImGui::Button("-", ImVec2(40, 40))) { state.skala_tekstu = (state.skala_tekstu > 0.5f) ? state.skala_tekstu - 0.1f : 0.5f; }
        ImGui::SameLine(); ImGui::Text(" %.1f ", state.skala_tekstu);
        ImGui::SameLine(); if (ImGui::Button("+", ImVec2(40, 40))) { state.skala_tekstu = (state.skala_tekstu < 3.5f) ? state.skala_tekstu + 0.1f : 3.5f; }

        ImGui::ColorEdit3(" Kolor tła", state.bgColor, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit3(" Kolor czcionki", state.textColor, ImGuiColorEditFlags_NoInputs);
        ImGui::Checkbox(" Statystyki", &state.show_debug_info);
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
        ImGui::Begin("Odczyt Lidar", &state.show_odczyt_lidar, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Tu bedzie wykres lidaru + odleglosc?");
        ImGui::Dummy(ImVec2(400, 300));
        ImGui::End();
    }
    // FPS i logi — mały overlay z historią komend, bo trzeba widzieć co się działo, nya
    if (state.show_debug_info) {
        ImGui::SetNextWindowPos(ImVec2(10, io.DisplaySize.y - 10), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
        ImGui::Begin("FPS_Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);
        for (const auto& log : state.historia_komend) {
            ImGui::Text("> %s", log.c_str());
        }
        ImGui::Separator();
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