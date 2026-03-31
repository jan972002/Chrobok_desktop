#pragma once
// Moduł GUI — rysujemy okienka i przyciski, wszystko cute i responsywne, uwu
// Created by jan97 on 08.01.2026. (komentarze przepuszczone przez kawaii filtr owo)
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "AppState.h"

class GuiModule {
public:
    static void Setup(GLFWwindow* window);
    static void RenderFrame(GLFWwindow* window, AppState& state, int monitorCount, GLFWmonitor** monitors);
    static void Shutdown();
    static float menuBarHeight;
private:
    static void WymusGraniceOkna(const char* name);
};



