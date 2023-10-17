#include "gui.h"
#include "imgui.h"
#include <cstdint>
#include <cstdio>

char strbuf[8];

void win_demo(bool* open) {ImGui::ShowDemoWindow(open);}

void win_regStat(bool* open) {
    ImGui::Begin("REGISTER STATUS", open);
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
}

void win_input(bool* open) {
    ImGui::Begin("INPUT", open);
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
}

void win_interPins(bool* open) {
    ImGui::Begin("INTERNAL PINS", open);
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
}

void win_ctl(bool* open) {
    ImGui::Begin("CONTROL", open);
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
}