cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(EMU_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/esm_emu)
set(GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/gui)

project(ESM VERSION 1.1)

set(EMU_FILES 
    ${EMU_DIR}/esmdebug.cpp)

set(IMGUI_FILES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

set(GUI_FILES
    ${SRC_DIR}/main_imgui.cpp
    ${SRC_DIR}/maingui.cpp
    ${GUI_DIR}/guistuff.cpp
    ${GUI_DIR}/guiwindows.cpp)

set(CLI_FILES 
    ${SRC_DIR}/maincli.cpp)

set(SRCS "")
list(APPEND SRCS ${EMU_FILES})
list(APPEND SRCS ${IMGUI_FILES})
list(APPEND SRCS ${GUI_FILES})
list(APPEND SRCS ${CLI})

add_executable(ESM ${SRC_DIR}/main.cpp)
target_sources(ESM PUBLIC ${SRCS})

target_link_directories(ESM PUBLIC
    ${EMU_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${GUI_DIR}
    ${SRC_DIR}
)

target_link_libraries(ESM PUBLIC
    ${EMU_FILES}
    ${IMGUI_FILES}
    ${GUI_FILES}
    ${CLI_FILES}
)

if(LINUX)
    message("\ntarget: linux\n")
    execute_process(COMMAND pkg-config --static --libs glfw3 RESULT_VARIABLE GLFW_LIBS)
    execute_process(COMMAND pkg-config --cflags glfw3 RESULT_VARIABLE GFFW_FLAGS)

    target_link_options(ESM PUBLIC ${GLFW_LIBS})
    target_compile_options(ESM PUBLIC ${GLFW_FLAGS})
endif()
