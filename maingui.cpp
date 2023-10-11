// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "esm.h"
#include <cstdint>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

ESM esm;
const char * regDesc[16] = {
    "TONE 1 CONFIG 1",
    "TONE 1 CONFIG 2",
    "TONE 2 CONFIG 1",
    "TONE 2 CONFIG 2",
    "TONE 3 CONFIG 1",
    "TONE 3 CONFIG 2",
    "TONE 4 CONFIG 1",
    "TONE 4 CONFIG 2",
    "TONE MASK",
    "TONE VOLUME",
    "SAMPLE PITCH LOW",
    "SAMPLE PITCH HIGH",
    "SAMPLE LOOP START LOW",
    "SAMPLE LOOP START HIGH",
    "SAMPLE LOOP END LOW",
    "SAMPLE LOOP END HIGH"
};

uint8_t u8_one = 1u;
uint8_t reg=0u, dat=0u;
char strbuf[8];
bool emuRunning = false;
bool emuRunningTemp = emuRunning;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ESM Test Tool v0.2", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(0.25f, 0.35f, 0.60f, 1.00f);

    esm.reset();

    // Main loop
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        glfwPollEvents();

        if (emuRunning) {
            esm.decodeRegister();
            esm.tick();
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("REGISTER STATUS");
            if(ImGui::BeginTable("REGISTERS",3)){
                ImGui::TableSetupColumn("REG",ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("VALUE",ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("DESC",ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableHeadersRow();
                for (uint8_t i=0; i<16; i++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    sprintf(strbuf, "%b", i);
                    ImGui::Text(strbuf);
                    ImGui::TableNextColumn();
                    sprintf(strbuf, "%b", esm.getRegStat(i));
                    ImGui::Text(strbuf);
                    ImGui::TableNextColumn();
                    ImGui::Text(regDesc[i]);
                }
                ImGui::EndTable();
            }

        ImGui::End();
        ImGui::Begin("INPUT");
            ImGui::InputScalar("REGISTER", ImGuiDataType_U8, &reg, &u8_one, NULL, "%u");
            ImGui::InputScalar("DATA", ImGuiDataType_U8, &dat, &u8_one, NULL, "%u");
            reg &= 0xf;
            // dat &= 0xff;
            if (ImGui::Button("WRITE")) {
                esm.writeRegBus(reg);
                esm.writeDataBus(dat);

                esm.decodeRegister();
            }
        ImGui::End();
        if (emuRunning) emuRunningTemp = true;
        ImGui::Begin("INTERNAL PINS");
            if (ImGui::BeginTable("INTERNAL PINS", 3)){
                ImGui::TableSetupColumn("",ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("BIN",ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("HEX",ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableHeadersRow();

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                ImGui::Text("TONE OUTPUTS");
                ImGui::TableNextColumn();
                ImGui::BeginDisabled();
                for (uint8_t i=8; i>0; i--) {
                    bool bits[8];
                    bits[i] = ((esm.getToneOut() >> (i-1)) & 1u) == 1;
                    ImGui::Checkbox("",&bits[i]);
                    ImGui::SameLine();
                }
                ImGui::EndDisabled();
                ImGui::TableNextColumn();
                sprintf(strbuf, "%x", esm.getToneOut());
                ImGui::Text(strbuf);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                ImGui::Text("PRNG OUTPUT");
                ImGui::TableNextColumn();
                ImGui::BeginDisabled();
                for (uint8_t i=8; i>0; i--) {
                    bool bits[8];
                    bits[i] = ((esm.getRandOut() >> (i-1)) & 1u) == 1;
                    ImGui::Checkbox("",&bits[i]);
                    ImGui::SameLine();
                }
                ImGui::EndDisabled();
                ImGui::TableNextColumn();
                sprintf(strbuf, "%x", esm.getRandOut());
                ImGui::Text(strbuf);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                ImGui::Text("PROGRAM COUNTER");
                ImGui::TableNextColumn();
                ImGui::BeginDisabled();
                for (uint8_t i=32; i>0; i--) {
                    bool bits[32];
                    bits[i] = ((esm.getPC() >> (i-1)) & 1u) == 1;
                    ImGui::Checkbox("",&bits[i]);
                    ImGui::SameLine();
                }
                ImGui::EndDisabled();
                ImGui::TableNextColumn();
                sprintf(strbuf, "%x", esm.getPC());
                ImGui::Text(strbuf);
                for (uint8_t a=0;a<4;a++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("LFSR#%d",a);
                    ImGui::TableNextColumn();
                    ImGui::BeginDisabled();
                    if (a < 2) {
                        for (uint8_t i=32; i>0; i--) {
                            bool bits[32];
                            bits[i] = ((esm.getLFSR(a) >> (i-1)) & 1u) == 1;
                            ImGui::Checkbox("",&bits[i]);
                            ImGui::SameLine();
                        }
                    } else {
                        for (uint8_t i=8; i>0; i--) {
                            bool bits[8];
                            bits[i] = ((esm.getLFSR(a) >> (i-1)) & 1u) == 1;
                            ImGui::Checkbox("",&bits[i]);
                            ImGui::SameLine();
                        }
                    }
                    ImGui::EndDisabled();
                    ImGui::TableNextColumn();
                    sprintf(strbuf, "%x", esm.getLFSR(a));
                    ImGui::Text(strbuf);
                }

                ImGui::EndTable();
            }
        ImGui::End();
        if (emuRunningTemp) emuRunning=true;
        ImGui::Begin("CONTROL");
            if (ImGui::Button("TICK")) {
                esm.decodeRegister();
                esm.tick();
            }
            ImGui::SameLine();
            ImGui::Checkbox("RUNNING", &emuRunning);
            emuRunningTemp=emuRunning;
            ImGui::SameLine();

            if (ImGui::Button("RESET")) esm.reset();
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
