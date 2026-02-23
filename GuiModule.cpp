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

void GuiModule::RenderFrame(GLFWwindow* window, AppState& state, int monitorCount, GLFWmonitor** monitors, Logic& logic) {
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
            ImGui::MenuItem("Sterowanie Kinematyka", nullptr, &state.show_sterowanie_kinematyka);
            ImGui::MenuItem("Parametry Ramienia", nullptr, &state.show_parametry_ramienia);
            ImGui::MenuItem("Sterowanie Ruchem", nullptr, &state.show_sterowanie_ruchem);
            ImGui::MenuItem("Obraz", nullptr, &state.show_detekcja_obrazu);
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
            ImGui::SliderInt(" M_1 ", &state.M_1, 500, 2380);
            ImGui::SliderInt(" M_2 ", &state.M_2, 0, 180);
            ImGui::SliderInt(" M_3 ", &state.M_3, 0, 180);
            ImGui::SliderInt(" M_4 ", &state.M_4, 34, 154);
            ImGui::SliderInt(" M_5 ", &state.M_5, 0, 180);
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            if (ImGui::Button("Zerowanie Ramienia")) {Logic::ZerowanieRamienia(state); }
        ImGui::End();
    }
    if (state.show_sterowanie_kinematyka) {
        WymusGraniceOkna("Sterowanie Kinematyka");
        ImGui::Begin("Sterowanie Kinematyka", &state.show_sterowanie_kinematyka, ImGuiWindowFlags_AlwaysAutoResize);
        float s = state.skala_tekstu;
        float padSize = 300.0f * s;
        ImVec2 p0 = ImGui::GetCursorScreenPos();
        ImVec2 sz = ImVec2(padSize, padSize);
        ImVec2 p1 = ImVec2(p0.x + sz.x, p0.y + sz.y);
        float yMax = state.dlugoscPodstawy + state.dlugoscPrzedramienia + 5;
        float yMin = -10.0f;
        float yRange = yMax - yMin;
        float xMax = state.dlugoscPodstawy + state.dlugoscPrzedramienia + 5;
        float yZeroRatio = yMax / yRange;
        float yZeroPos = p0.y + (sz.y * yZeroRatio);
        float maxZasieg = (float)(state.dlugoscPodstawy + state.dlugoscPrzedramienia);
        float unitToPxX = sz.x / xMax;
        float unitToPxY = sz.y / yRange;
        float promienPxX = maxZasieg * unitToPxX;
        float promienPxY = maxZasieg * unitToPxY;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // Tlo
        draw_list->AddRectFilled(p0, p1, IM_COL32(60, 20, 20, 255));
        draw_list->AddRect(p0, p1, IM_COL32(100, 100, 100, 255));
        ImVec2 srodekZasiegu = ImVec2(p0.x, yZeroPos);
        draw_list->PushClipRect(p0, p1, true);

        // Obszar roboczy I zasieg
        draw_list->PathLineTo(srodekZasiegu);
        for (float a = -1.5708f; a <= 1.5708f; a += 0.05f) {
            draw_list->PathLineTo(ImVec2(
                srodekZasiegu.x + cosf(a) * promienPxX,
                srodekZasiegu.y + sinf(a) * promienPxY
            ));
        }
        draw_list->PathFillConvex(IM_COL32(30, 30, 30, 255));
        for (float a = -1.5708f; a <= 1.5708f; a += 0.05f) {
            draw_list->PathLineTo(ImVec2(
                srodekZasiegu.x + cosf(a) * promienPxX,
                srodekZasiegu.y + sinf(a) * promienPxY
            ));
        }
        draw_list->PathStroke(IM_COL32(255, 50, 50, 200), 0, 2.0f * s);
        draw_list->PopClipRect();
        draw_list->AddLine(ImVec2(p0.x, yZeroPos), ImVec2(p1.x, yZeroPos), IM_COL32(70, 70, 70, 255), 1.0f);

        // Lokiec
        float L1 = (float)state.dlugoscPodstawy;
        float L2 = (float)state.dlugoscPrzedramienia;
        float d2 = (state.wspolrzednaX * state.wspolrzednaX) + (state.wspolrzednaY * state.wspolrzednaY);
        float d = std::sqrt(d2);
        float gamma = std::atan2(state.wspolrzednaY, state.wspolrzednaX);
        float argBeta = (L1 * L1 + d2 - L2 * L2) / (2.0f * L1 * d);
        float beta = std::acos(std::clamp(argBeta, -1.0f, 1.0f));
        float elbowX = L1 * cosf(gamma + beta);
        float elbowY = L1 * sinf(gamma + beta);

        ImVec2 pBase  = srodekZasiegu;
        ImVec2 pElbow = ImVec2(p0.x + elbowX * unitToPxX, p0.y + (yMax - elbowY) * unitToPxY);
        ImVec2 pEnd   = ImVec2(p0.x + state.wspolrzednaX * unitToPxX, p0.y + (yMax - state.wspolrzednaY) * unitToPxY);
        float grubosc = 4.0f * s;
        ImU32 colArm = IM_COL32(200, 200, 200, 255);
        draw_list->AddLine(pBase, pElbow, colArm, grubosc);
        draw_list->AddLine(pElbow, pEnd, colArm, grubosc);
        draw_list->AddCircleFilled(pElbow, 5.0f * s, IM_COL32(80, 80, 80, 255));

        // Trojkat
        ImVec2 t1 = ImVec2(p0.x + (0.0f * unitToPxX), p0.y + (yMax - 0.0f) * unitToPxY);
        ImVec2 t2 = ImVec2(p0.x + (5.0f * unitToPxX), p0.y + (yMax - (-5.0f)) * unitToPxY);
        ImVec2 t3 = ImVec2(p0.x + (0.0f * unitToPxX), p0.y + (yMax - (-5.0f)) * unitToPxY);
        draw_list->AddTriangleFilled(t1, t2, t3, IM_COL32(255, 165, 0, 60));
        draw_list->AddTriangle(t1, t2, t3, IM_COL32(255, 165, 0, 200), 2.0f * s);

        // Prostokat
        ImVec2 rec0 = ImVec2(p0.x + (0.0f * unitToPxX),  p0.y + (yMax - (-5.0f)) * unitToPxY);
        ImVec2 rec1 = ImVec2(p0.x + (15.0f * unitToPxX), p0.y + (yMax - (-10.0f)) * unitToPxY);
        draw_list->AddRectFilled(rec0, rec1, IM_COL32(255, 165, 0, 60));
        draw_list->AddRect(rec0, rec1, IM_COL32(255, 165, 0, 200), 0.0f, 0, 2.0f * s);

        // Joystick
        ImGui::InvisibleButton("##joystick", sz);
        if (ImGui::IsItemActive()) {
            ImVec2 m = ImGui::GetIO().MousePos;
            float tx = ((m.x - p0.x) / sz.x) * xMax;
            float ty = yMax - ((m.y - p0.y) / sz.y) * yRange;
            tx = std::clamp(tx, 0.0f, xMax);
            ty = std::clamp(ty, yMin, yMax);

            // Kolizja kolo
            float distSq = (tx * tx) + (ty * ty);
            if (distSq > maxZasieg * maxZasieg) {
                float dist = std::sqrt(distSq);
                tx = (tx / dist) * maxZasieg;
                ty = (ty / dist) * maxZasieg;
            }

            // Kolizja prostokat
            if (tx <= 15.0f && ty <= -5.0f) {
                if ((15.0f - tx) < (ty - (-5.0f))) tx = 15.01f;
                else ty = -4.99f;
            }
            // Kolizja trojkat
            if (tx <= 5.0f && ty <= 0.0f && ty >= -tx) {
                if (tx < -ty) tx = -ty + 0.01f;
                else ty = -tx + 0.01f;
            }
            state.wspolrzednaX = tx;
            state.wspolrzednaY = ty;
            logic.CalculateKinematics(state);
        }
        ImGui::Dummy(ImVec2(padSize, 5.0f * s));
        // Kursor
        float dotX = p0.x + (state.wspolrzednaX * unitToPxX);
        float dotY = p0.y + (yMax - state.wspolrzednaY) * unitToPxY;
        draw_list->AddCircleFilled(ImVec2(dotX, dotY), 8.0f * s, IM_COL32(66, 150, 250, 255));

        // Suwak lukowy
        ImGui::Spacing();
        float arcRadius = 60.0f * s;
        float arcThickness = 12.0f * s;
        ImVec2 arcCenter = ImVec2(p0.x + sz.x / 2.0f, ImGui::GetCursorScreenPos().y + arcRadius + 10.0f * s);
        ImGui::SetCursorScreenPos(ImVec2(arcCenter.x - arcRadius - arcThickness, arcCenter.y - arcRadius - arcThickness));
        ImGui::InvisibleButton("##arc_slider", ImVec2((arcRadius + arcThickness) * 2, arcRadius + arcThickness));

        // Tekst
        char buf[32];
        sprintf(buf, "%d°", state.M_5);
        ImVec2 textSize = ImGui::CalcTextSize(buf);
        ImVec2 textPos = ImVec2(
            arcCenter.x - (textSize.x / 2.0f),
            arcCenter.y - (arcRadius * 0.4f)
        );
        draw_list->AddText(textPos, IM_COL32(255, 255, 255, 255), buf);

        //Luk
        if (ImGui::IsItemActive()) {
            ImVec2 m = ImGui::GetIO().MousePos;
            float dx = m.x - arcCenter.x;
            float dy = m.y - arcCenter.y;
            if (dy >= 0) {
                state.M_5 = (dx > 0) ? 180 : 0;
            } else {
                float angle = atan2f(dy, dx);
                float t = (angle + 3.1415f) / 3.1415f;
                state.M_5 = (int)(std::clamp(t, 0.0f, 1.0f) * 180.0f);
            }
        }

        // Rysowanie polkola
        draw_list->PathArcTo(arcCenter, arcRadius, -3.1415f, 0.0f, 32);
        draw_list->PathStroke(IM_COL32(50, 50, 50, 255), 0, arcThickness);
        float currentAngle = -3.1415f + (state.M_5 / 180.0f) * 3.1415f;
        ImVec2 handlePos = ImVec2(arcCenter.x + cosf(currentAngle) * arcRadius, arcCenter.y + sinf(currentAngle) * arcRadius);
        draw_list->AddCircleFilled(handlePos, (arcThickness / 2.0f) + 2.0f * s, IM_COL32(255, 255, 255, 255));
        ImGui::SetCursorScreenPos(ImVec2(p0.x, arcCenter.y + 10.0f * s));
        ImGui::Dummy(ImVec2(padSize, 5.0f * s));


        ImGui::Separator();
        ImGui::Spacing();
        ImGui::SetNextItemWidth(padSize);
        ImGui::PushID("nadgarstek");
        ImGui::SliderInt("##", &state.M_2, 0, 180);
        ImGui::PopID();
        ImGui::Spacing();
        if (ImGui::Button("Otworz", ImVec2(padSize/2 - 5, 50))) {
            state.M_1 = 500;
        };
        ImGui::SameLine();
        if (ImGui::Button( "Zamknij", ImVec2(padSize/2 - 5, 50))) {
            state.M_1 = 2380;
        };
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Pozycja: X: %.1f | Y: %.1f", state.wspolrzednaX, state.wspolrzednaY);
        if (ImGui::Button("Reset", ImVec2(padSize, 40 * s))) {
            Logic::ZerowanieRamienia(state);
        }

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
        ImGui::Begin("O Nas", &state.show_aboutUs_info,  ImGuiWindowFlags_NoCollapse);
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
        ImGui::Begin("Ruch", &state.show_sterowanie_ruchem, ImGuiWindowFlags_AlwaysAutoResize);
        int stale_x = 100; int stale_y = 60;
        ImGui::Dummy(ImVec2(stale_x, stale_y)); ImGui::SameLine();
        ImGui::Button(ICON_FA_ARROW_UP "##Przod", ImVec2(stale_x, stale_y));
        state.ruch_przod = ImGui::IsItemActive();
        ImGui::Button(ICON_FA_ARROW_LEFT "##Lewo", ImVec2(stale_x, stale_y));
        state.ruch_lewo = ImGui::IsItemActive();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STOP "##Stop", ImVec2(stale_x, stale_y))) {
            state.zatrzymanie = true;
        }
        ImGui::SameLine();
        ImGui::Button(ICON_FA_ARROW_RIGHT "##Prawo", ImVec2(stale_x, stale_y));
        state.ruch_prawo = ImGui::IsItemActive();
        ImGui::Dummy(ImVec2(stale_x, stale_y)); ImGui::SameLine();
        ImGui::Button(ICON_FA_ARROW_DOWN "##Tyl", ImVec2(stale_x, stale_y));
        state.ruch_tyl = ImGui::IsItemActive();
        ImGui::Spacing();
        if (ImGui::Button(ICON_FA_ROTATE_LEFT "##OBR.L", ImVec2(stale_x, stale_y))) {}
        state.skret_lewo = ImGui::IsItemActive();
        ImGui::SameLine(); ImGui::Dummy(ImVec2(stale_x, stale_y)); ImGui::SameLine();
        if (ImGui::Button(ICON_FA_ROTATE_RIGHT "##OBR.P", ImVec2(stale_x, stale_y))) {}
        state.skret_prawo = ImGui::IsItemActive();
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
                    char buf[128];
                    snprintf(buf, sizeof(buf), "%s (Monitor %d)##%d", glfwGetMonitorName(monitors[n]), n + 1, n);
                    if (ImGui::Selectable(buf, state.selected_monitor == n)) {
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
            if (ImGui::Button("-", ImVec2(20 * state.skala_tekstu, 20 * state.skala_tekstu))) { if (state.skala_tekstu > 0.5f) state.skala_tekstu -= 0.1f; }
            ImGui::SameLine(); ImGui::Text(" %.1f ", state.skala_tekstu);
            ImGui::SameLine(); if (ImGui::Button("+", ImVec2(20 * state.skala_tekstu, 20 * state.skala_tekstu))) { if (state.skala_tekstu < 3.5f) state.skala_tekstu += 0.1f; }

            ImGui::ColorEdit3(" Kolor tła", state.bgColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit3(" Kolor czcionki", state.textColor, ImGuiColorEditFlags_NoInputs);
            ImGui::Checkbox(" Statystyki", &state.show_debug_info);
            if (ImGui::Checkbox(" V-SYNC", &state.vsync_state)) {
                glfwSwapInterval(state.vsync_state);
            }
            if (ImGui::Checkbox(" Polacz z robotem", &state.is_usb_connected)) {
                if (state.is_usb_connected) {
                    if (!logic.ConnectUSB(state)) {
                        state.is_usb_connected = false;
                    }
                }
                else {
                    logic.DisconnectUSB(state);
                }
            }
            ImGui::SameLine();
            static std::vector<std::string> porty;
            if (porty.empty()) {
                porty = logic.GetAvailableComPorts();
            }
            const char* current_port = state.portName.c_str();
            ImGui::SetNextItemWidth(150.0f);
            if (ImGui::BeginCombo("##WybierzPortCombo", current_port)) {
                for (int n = 0; n < (int)porty.size(); n++) {
                    bool is_selected = (state.portName == porty[n]);
                    if (ImGui::Selectable(porty[n].c_str(), is_selected)) {
                        state.portName = porty[n];
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_REFRESH"##Odsniez Listy")) {
                porty = logic.GetAvailableComPorts();
            }
            if (ImGui::Checkbox(" Polacz z siecia", &state.is_network_connected)) {
                if (state.is_network_connected) {
                    logic.ConnectToNetwork(state);
                    state.is_network_connected = true;
                }
                else {
                    logic.DisconnectNetwork();
                    state.is_network_connected = false;
                }
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                ImGui::BeginTooltip();
                ImGui::TextUnformatted("Wymaga połączenia z siecią WiFi");
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Szukaj sieci z nazwą 'Chrobok...'");
                ImGui::EndTooltip();
            }
            ImGui::Text("Aktywna Sieć: %s", state.SSID.c_str());
        ImGui::End();
    }


    if (state.show_detekcja_obrazu) {
        WymusGraniceOkna("Obraz");
        ImGuiWindow* window = ImGui::FindWindowByName("Obraz");
        float aspect_ratio = 4.0f / 3.0f;
        float v_overhead = 85.0f;
        if (window) {
            float current_w = window->Size.x;
            float ideal_h = ((current_w - 20.0f) / aspect_ratio) + v_overhead;
            ImGui::SetNextWindowSize(ImVec2(current_w, ideal_h));
        }
        ImGui::Begin("Obraz", &state.show_detekcja_obrazu, ImGuiWindowFlags_NoScrollbar);
        float start_stop_w = 110.0f;
        if (state.camera_is_running) {
            if (ImGui::Button("STOP", ImVec2(start_stop_w, 0))) state.camera_is_running = false;
        } else {
            if (ImGui::Button("START", ImVec2(start_stop_w, 0))) state.camera_is_running = true;
        }
        ImGui::SameLine();

        static std::vector<std::string> available_cams;
        float refresh_w = 80.0f;
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - refresh_w - spacing);
        std::string preview = (state.selected_camera_index < available_cams.size()) ? available_cams[state.selected_camera_index] : "Wybierz...";
        if (ImGui::BeginCombo("##source", preview.c_str())) {
            for (int n = 0; n < (int)available_cams.size(); n++) {
                if (ImGui::Selectable(available_cams[n].c_str(), state.selected_camera_index == n)) {
                    state.selected_camera_index = n;
                    state.camera_needs_reset = true;
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        if (available_cams.empty() || ImGui::Button(ICON_FA_REFRESH"##Odswiez", ImVec2(refresh_w, 0))) {
            available_cams.clear();
            cv::VideoCapture temp_cap;
            for (int i = 0; i < 4; i++) {
                if (temp_cap.open(i, cv::CAP_DSHOW)) {
                    available_cams.push_back("Kamera " + std::to_string(i));
                    temp_cap.release();
                }
            }
            if (available_cams.empty()) available_cams.push_back("Brak kamer");
        }
        ImGui::Separator();
        ImVec2 content_size = ImGui::GetContentRegionAvail();
        if (state.camera_is_running && state.cameraTexture != 0) {
            ImGui::Image((void*)(intptr_t)state.cameraTexture, content_size);
        } else {
            ImGui::BeginChild("Placeholder", content_size, true);
                ImGui::SetCursorPos(ImVec2(content_size.x * 0.5f - 50, content_size.y * 0.5f - 10));
                ImGui::Text("KAMERA WYŁĄCZONA");
            ImGui::EndChild();
        }
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
        for (const auto& log : state.historia_komend) {
            ImGui::Text("> %s", log.c_str());
        }
        ImGui::Separator();
        const GLubyte* renderer = glGetString(GL_RENDERER);
        ImGui::Text("%s", renderer);
        ImGui::SameLine();
        ImGui::Text("| %.3f ms | %.1f FPS", 1000.0f / io.Framerate, io.Framerate);
        ImGui::SameLine();
        ImGui::End();
    }

    ImGui::Render();
}

void GuiModule::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}