#include "esmdebug.h"
#include <cstdint>
#include <string>

int start_gui();

int run_cli();
int run_gui();

extern ESM esm;
extern uint8_t reg, dat;

std::string toBin(uint32_t n);
