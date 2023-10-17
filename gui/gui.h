#include <cstdint>
#include "esmdebug.h"

extern const char * regDesc[16];


extern uint8_t u8_one;
extern uint8_t reg, dat;
extern bool emuRunning;
extern bool emuRunningTemp;

extern bool open_regStat;
extern bool open_input;
extern bool open_interPins;
extern bool open_ctl;
extern bool open_demo;

extern ESM esm;

void win_regStat(bool* open);
void win_input(bool* open);
void win_interPins(bool* open);
void win_ctl(bool* open);
void win_demo (bool* open);