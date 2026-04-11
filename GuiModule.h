//
// Created by jan97 on 08.01.2026.
//
#pragma once
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "AppState.h"
#include "Logic.h"
class GuiModule {
public:
    static void Setup(GLFWwindow* window);
    static void RenderFrame(GLFWwindow* window, AppState& state, int monitorCount, GLFWmonitor** monitors, Logic& logic);
    static void Shutdown();
    static float menuBarHeight;
private:
    static void WymusGraniceOkna(const char* name);
};



