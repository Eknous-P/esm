#include "gui.h"
#include <cstdint>

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
uint8_t reg = 0, dat = 0;

bool emuRunning = false;
bool emuRunningTemp = emuRunning;

bool open_regStat = false;
bool open_input = false;
bool open_interPins = false;
bool open_ctl = false;
bool open_demo = false;

ESM esm;