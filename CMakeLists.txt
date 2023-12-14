cmake_minimum_required(VERSION 3.25)
find_package(PkgConfig)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(EMU_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/esm_emu)
set(GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/gui)

set(FLAGS)
list(APPEND FLAGS -g)
list(APPEND FLAGS -Wall)
list(APPEND FLAGS -Wformat)

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

set(MAINFILE
    ${SRC_DIR}/main.cpp)

set(SRCS)

list(APPEND SRCS ${EMU_FILES})
list(APPEND SRCS ${IMGUI_FILES})
list(APPEND SRCS ${GUI_FILES})
list(APPEND SRCS ${CLI_FILES})
list(APPEND SRCS ${MAINFILE})

add_executable(ESM ${SRCS})

target_include_directories(ESM SYSTEM PRIVATE
    ${EMU_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${GUI_DIR}
    ${SRC_DIR}
)

# target_link_libraries(ESM PUBLIC
#     ${EMU_FILES}
#     ${IMGUI_FILES}
#     ${GUI_FILES}
#     ${CLI_FILES}
# )

if(LINUX)
    message("\ntarget: linux\n")

    # set(LINUX_GL_LIBS -lGL)
    # execute_process(COMMAND pkg-config --static --libs glfw3 OUTPUT_VARIABLE GLFW_LIBS)
    # execute_process(COMMAND pkg-config --cflags glfw3 OUTPUT_VARIABLE GLFW_FLAGS)
    # list(APPEND LINUX_GL_LIBS ${GLFW_LIBS})
    # list(APPEND FLAGS ${GLFW_FLAGS})
    pkg_check_modules(GLFW REQUIRED glfw3)

    # target_link_options(ESM PRIVATE ${LINUX_GL_LIBS})
endif()

add_compile_options(${FLAGS})
