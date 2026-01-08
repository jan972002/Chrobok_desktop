#include "GuiModule.h"
#include "ConfigManager.h"
#include <GLFW/glfw3.h>
#include "imgui_impl_opengl3.h"
void test() {

}
int main() {
    if (!glfwInit()) return -1;

    AppState state;
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
    glfwSwapInterval(1);

    GuiModule::Setup(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;

        GuiModule::RenderFrame(window, state, monitorCount, monitors);

        int dw, dh; glfwGetFramebufferSize(window, &dw, &dh);
        glViewport(0, 0, dw, dh);
        glClearColor(state.bgColor[0], state.bgColor[1], state.bgColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ConfigManager::Zapisz(state);
    GuiModule::Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

