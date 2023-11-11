#include <cstdint>
#include "esmdebug.h"
#include "imgui.h"

extern const char * regDesc[16];


extern uint8_t u8_one;
extern uint8_t reg, dat;
extern bool emuRunning;

extern bool open_regStat;
extern bool open_input;
extern bool open_interPins;
extern bool open_ctl;
extern bool open_demo;
extern bool open_osc;
extern bool open_playg;

extern ESM esm;

extern float outBuf[128];
extern float outBuf4x[512];

void win_regStat(bool* open);
void win_input(bool* open);
void win_interPins(bool* open);
void win_ctl(bool* open);
void win_demo(bool* open);
void win_osc(bool* open);
void win_playground(bool* open);
void bufferUpdate();

void obj_bit(float width, float height, bool state, ImVec2 org);