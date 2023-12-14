#include "gui.h"
#include "main.h"
#include <cstdint>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif


int run_gui() {
        if (emuRunning) {
            esm.decodeRegister();
            esm.tick();
            bufferUpdate();
        }

        if (open_regStat) win_regStat(&open_regStat);
        if (open_input) win_input(&open_input);
        if (open_interPins) win_interPins(&open_interPins);
        if (open_ctl) win_ctl(&open_ctl);
        if (open_demo) win_demo(&open_demo);
        if (open_osc) win_osc(&open_osc);
        if (open_playg) win_playground(&open_playg);

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Windows")) {
                ImGui::MenuItem("REGISTER STATUS", NULL, &open_regStat);
                ImGui::MenuItem("INPUT", NULL, &open_input);
                ImGui::MenuItem("INTERNAL PINS", NULL, &open_interPins);
                ImGui::MenuItem("CONTROL", NULL, &open_ctl);
                ImGui::MenuItem("DEMO", NULL, &open_demo);
                ImGui::MenuItem("OSC", NULL, &open_osc);
                ImGui::MenuItem("PLAYGROUND", NULL, &open_playg);

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

    return 0;
}
