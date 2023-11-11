cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR "src")
set(IMGUI_DIR "imgui")
set(EMU_DIR "$src/esm_emu")
set(GUI_DIR "$src/gui")

set(LIBS_CLFLAGS)

project(ESM VERSION 1.1)

set(EMU_FILES
    "${EMU_DIR}/esm_debug.cpp")

set(IMGUI_FILES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp")

set(GUI_FILES
    "${SRC_DIR}/main_imgui.cpp"
    "${SRC_DIR}/maingui.cpp"
    "${GUI_DIR}/guistuff.cpp"
    "${GUI_DIR}/guiwindows.cpp")

set(CLI_FILES 
    "${SRC_DIR}/maincli.cpp")

add_executable(ESM "${SRC_DIR}/main.cpp")

target_link_libraries(ESM PUBLIC 
    "${ESM_FILES}"
    "${IMGUI_FILES}"
    "${GUI_FILES}"
    "${CLI_FILES}")

target_include_directories(ESM PRIVATE
    "${EMU_DIR}"
    "${IMGUI_DIR}"
    "${GUI_DIR}"
    "{PROJECT_BINARY_DIR}")

if(LINUX)
    message("target: linux")
    add_definitions("-lGL `pkg-config --static --libs glfw3`")
    target_compile_definitions(ESM PRIVATE "`pkg-config --cflags glfw3`")
endif()
