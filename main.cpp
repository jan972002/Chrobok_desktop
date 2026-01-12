#include "GuiModule.h"
#include "ConfigManager.h"
#include <GLFW/glfw3.h>
#include "imgui_impl_opengl3.h"
#include "Logic.h"
#include <thread>
#include "AppState.h"

int main() {
    if (!glfwInit()) return -1;

    AppState state;
    Logic logic;
    ConfigManager::Laduj(state);

    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    if (state.selected_monitor >= monitorCount) state.selected_monitor = 0;

    GLFWmonitor* targetMonitor = monitors[state.selected_monitor];
    const GLFWvidmode* mode = glfwGetVideoMode(targetMonitor);
    int mX, mY; glfwGetMonitorPos(targetMonitor, &mX, &mY);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Robot Control", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, mX, mY);
    glfwSwapInterval(state.vsync_state);

    GuiModule::Setup(window);

    state.is_running = true;
    std::thread logicThread(&Logic::ParseCommand, &logic, std::ref(state));
    logicThread.detach();

    std::thread cameraThread(&Logic::CameraLoop, &logic, std::ref(state));
    cameraThread.detach();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;

        logic.UpdateTexture(state);

        GuiModule::RenderFrame(window, state, monitorCount, monitors, logic);

        int dw, dh; glfwGetFramebufferSize(window, &dw, &dh);
        glViewport(0, 0, dw, dh);
        glClearColor(state.bgColor[0], state.bgColor[1], state.bgColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    state.is_running = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    ConfigManager::Zapisz(state);
    GuiModule::Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}