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
        ImVec2 p;
        float x;
        const float width = 5.0f;
        const float height = 15.0f;
        const float dist = width * 1.5f;
        if (ImGui::BeginTable( "INTERNAL PINS", 3)){
            ImGui::TableSetupColumn("",ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("BIN",ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("HEX",ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("TONE OUTPUTS");
            ImGui::TableNextColumn();
            p = ImGui::GetCursorScreenPos();
            x = p.x;
            for (uint8_t i=8; i>0; i--) {
                bool bits[8];
                bits[i] = ((esm.getToneOut() >> (i-1)) & 1u) == 1;
                obj_bit(width,height,bits[i],ImVec2(x,p.y));
                x += dist;
            }
            ImGui::TableNextColumn();
            sprintf(strbuf, "%x", esm.getToneOut());
            ImGui::Text(strbuf);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("PRNG OUTPUT");
            ImGui::TableNextColumn();
            p = ImGui::GetCursorScreenPos();
            x = p.x;
            for (uint8_t i=6; i>0; i--) {
                bool bits[6];
                bits[i] = ((esm.getRandOut() >> (i-1)) & 1u) == 1;
                obj_bit(width,height,bits[i],ImVec2(x,p.y));
                x += dist;
            }
            ImGui::TableNextColumn();
            sprintf(strbuf, "%x", esm.getRandOut());
            ImGui::Text(strbuf);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("PROGRAM COUNTER");
            ImGui::TableNextColumn();
            p = ImGui::GetCursorScreenPos();
            x = p.x;
            for (uint8_t i=32; i>0; i--) {
                bool bits[32];
                bits[i] = ((esm.getPC() >> (i-1)) & 1u) == 1;
                obj_bit(width,height,bits[i],ImVec2(x,p.y));
                x += dist;
            }
            ImGui::TableNextColumn();
            sprintf(strbuf, "%x", esm.getPC());
            ImGui::Text(strbuf);
            for (uint8_t a=0;a<4;a++) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                ImGui::Text("LFSR#%d",a);
                ImGui::TableNextColumn();
                if (a < 2) {
                    p = ImGui::GetCursorScreenPos();
                    x = p.x;
                    for (uint8_t i=32; i>0; i--) {
                        bool bits[32];
                        bits[i] = ((esm.getLFSR(a) >> (i-1)) & 1u) == 1;        
                        obj_bit(width,height,bits[i],ImVec2(x,p.y));
                        x += dist;
                    }
                } else {
                    p = ImGui::GetCursorScreenPos();
                    x = p.x;
                    for (uint8_t i=8; i>0; i--) {
                        bool bits[8];
                        bits[i] = ((esm.getLFSR(a) >> (i-1)) & 1u) == 1;        
                        obj_bit(width,height,bits[i],ImVec2(x,p.y));
                        x += dist;
                    }
                }
                ImGui::TableNextColumn();
                sprintf(strbuf, "%x", esm.getLFSR(a));
                ImGui::Text(strbuf);
            }
            for (uint8_t a=0;a<4;a++) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                ImGui::Text("TONE COUNTER #%d",a);
                ImGui::TableNextColumn();
                p = ImGui::GetCursorScreenPos();
                x = p.x;
                for (uint8_t i=12; i>0; i--) {
                    bool bits[12];
                    bits[i] = ((esm.getToneCounter(a) >> (i-1)) & 1u) == 1;    
                    obj_bit(width,height,bits[i],ImVec2(x,p.y));
                    x += dist;
                }
                ImGui::TableNextColumn();
                sprintf(strbuf, "%x", esm.getToneCounter(a));
                ImGui::Text(strbuf);
            }
            for (uint8_t a=0;a<4;a++) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                ImGui::Text("TONE CONFIG #%d",a);
                ImGui::TableNextColumn();
                p = ImGui::GetCursorScreenPos();
                x = p.x;
                for (uint8_t i=16; i>0; i--) {
                    bool bits[16];
                    bits[i] = ((esm.getToneConf(a) >> (i-1)) & 1u) == 1;    
                    obj_bit(width,height,bits[i],ImVec2(x,p.y));
                    x += dist;
                }
                ImGui::TableNextColumn();
                sprintf(strbuf, "%x", esm.getToneConf(a));
                ImGui::Text(strbuf);
            }
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("SAMPLE COUNTER");
            ImGui::TableNextColumn();
            p = ImGui::GetCursorScreenPos();
            x = p.x;
            for (uint8_t i=16; i>0; i--) {
                bool bits[16];
                bits[i] = ((esm.getSampleAddrBus() >> (i-1)) & 1u) == 1;
                obj_bit(width,height,bits[i],ImVec2(x,p.y));
                x += dist;
            }
            ImGui::TableNextColumn();
            sprintf(strbuf, "%x", esm.getSampleAddrBus());
            ImGui::Text(strbuf);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("PSG OUTPUTS");
            ImGui::TableNextColumn();
            p = ImGui::GetCursorScreenPos();
            x = p.x;
            for (uint8_t i=4; i>0; i--) {
                bool bits[4];
                bits[i] = ((esm.getPSGOut() >> (i-1)) & 1u) == 1;
                obj_bit(width,height,bits[i],ImVec2(x,p.y));
                x += dist;
            }
            ImGui::TableNextColumn();
            sprintf(strbuf, "%x", esm.getPSGOut());
            ImGui::Text(strbuf);
        
            ImGui::EndTable();
        }
    ImGui::End();
}

void win_ctl(bool* open) {
    ImGui::Begin("CONTROL", open);
        if (ImGui::Button("TICK")) {
            esm.decodeRegister();
            esm.tick();
            bufferUpdate();
        }
        ImGui::SameLine();
        ImGui::Checkbox("RUNNING", &emuRunning);
        ImGui::SameLine();
        if (ImGui::Button("RESET")) esm.reset();
    ImGui::End();
}

void obj_bit(float width, float height, bool state, ImVec2 org) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    const ImU32 cl = ImGui::GetColorU32(IM_COL32(state ? 0 : 255, state ? 255 : 0, 0, 255));
    drawList->AddRectFilled(ImVec2((org.x),(org.y)), ImVec2((org.x+width),(org.y+height)), cl, 1.0f);
}

void bufferUpdate() {
    for (uint16_t i=IM_ARRAYSIZE(outBuf)-1; i>0; i--) { //this is probably way too slow
        outBuf[i] = outBuf[i-1];
        outBuf4x[4*i] = outBuf[i];
        outBuf4x[4*i+1] = outBuf[i];
        outBuf4x[4*i+2] = outBuf[i];
        outBuf4x[4*i+3] = outBuf[i];
    }
    outBuf[0] = esm.getAnalogOut();
}

void win_osc(bool* open) {
    ImGui::Begin("OSCILLOSCOPE", open);
    ImGui::PlotLines("", outBuf4x, IM_ARRAYSIZE(outBuf4x), 0, NULL, 0.0f, 4.0f, ImGui::GetContentRegionAvail());
    ImGui::End();
}
