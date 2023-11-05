cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(IMGUI_DIR "${PROJECT_BINARY_DIR}/imgui")
set(EMU_DIR "${PROJECT_BINARY_DIR}/esm_emu")
set(GUI_DIR "${PROJECT_BINARY_DIR}/gui")

project(ESM VERSION 1.1)

# add_library(imgui imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp ./backends/imgui_impl_glfw.cpp ./backends/imgui_impl_opengl3.cpp)
# add_subdirectory(imgui)

# add_library(gui guistuff.cpp guiwindows.cpp)
# add_subdirectory(gui)

# add_library(emu esmdebug.cpp)
# add_subdirectory(emu)

# add_executable(esmtestgui maingui.cpp)

# target_link_libraries(esmtestgui imgui gui emu)

target_include_directories(esmtestgui PUBLIC
 ${IMGUI_DIR}
 ${EMU_DIR}
 ${GUI_DIR}
 )
