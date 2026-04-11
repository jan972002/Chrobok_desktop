cmake_minimum_required(VERSION 3.20...3.31)
project(Chrobok_desktop CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(OpenCV_DIR "D:/Biblioteki_C++/OpenCV/4.10.0")
find_package(OpenCV REQUIRED)


include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        docking
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    AppState.h
    ConfigManager.h
    GuiModule.h
    GuiModule.cpp
    Icons.h
    Logic.h
    Logic.cpp
)


add_executable(Chrobok_desktop main.cpp ${IMGUI_SOURCES}) # WIN32 dla braku cmd

target_include_directories(Chrobok_desktop PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${OpenCV_INCLUDE_DIRS}
    "D:/Biblioteki_C++/OpenCV/4.10.0/include"
)

# Łączenie bibliotek
if(WIN32)
    target_link_libraries(Chrobok_desktop PRIVATE
        glfw
        opengl32
        ${OpenCV_LIBS}
        ws2_32
    )
else()
    target_link_libraries(Chrobok_desktop PRIVATE
        glfw
        GL
        ${OpenCV_LIBS}
    )
endif()

if(WIN32)
    get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    set(DLLS_TO_COPY
        "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
        "${MINGW_BIN_DIR}/libstdc++-6.dll"
        "${MINGW_BIN_DIR}/libwinpthread-1.dll"
        "${OpenCV_DIR}/x64/mingw/bin/libopencv_world4100.dll"
        "${OpenCV_DIR}/x64/mingw/bin/opencv_videoio_ffmpeg4100_64.dll"
    )
    add_custom_command(TARGET Chrobok_desktop POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLLS_TO_COPY}
        $<TARGET_FILE_DIR:Chrobok_desktop>
    )
endif()