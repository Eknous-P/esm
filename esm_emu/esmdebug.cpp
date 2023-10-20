#include <cstdint>
#include "esmdebug.h"

#define EMU_VERSION "v1"

/*====REGISTERS (once more)====

REG=|NAME======================|DATA====
----|--------------------------|7------0
0000|TONE 1 CONFIG 1 ==========|FFFFFFFF
0001|TONE 1 CONFIG 2 ==========|VVVVFFFF
0010|TONE 2 CONFIG 1 ==========|FFFFFFFF
0011|TONE 2 CONFIG 2 ==========|VVVVFFFF
0100|TONE 3 CONFIG 1 ==========|FFFFFFFF
0101|TONE 3 CONFIG 2 ==========|VVVVFFFF
0110|TONE 4 CONFIG 1 ==========|FFFFFFFF
0111|TONE 4 CONFIG 2 ==========|VVVVFFFF
1000|GLOBAL TONE GONFIG (MASK)=|NNNNDDDD
1001|GLOBAL VOLUME ============|VVVVVVVV
1010|SAMPLE PITCH LOW + PLAYING|FFFFFFFB
1011|SAMPLE PITCH HIGH ========|FFFFFFFF
1100|SAMPLE LOOP START LOW ====|AAAAAAAA
1101|SAMPLE LOOP START HIGH ===|AAAAAAAA
1110|SAMPLE LOOP END LOW ======|AAAAAAAA
1111|SAMPLE LOOP END HIGH =====|AAAAAAAA
---------------------------------------
(F)requency    (D)ivider
(V)olume       (A)ddress
(N)oise        (B)Playing

*/

void ESM::decodeRegister() {
    esm.regStat[esm.regBus] = esm.dataBus;
    switch (esm.regBus % 16) {
        case 0: case 2: case 4: case 6: {
            esm.toneConf[esm.regBus >> 1] &= 0xff00;//erase low byte
            esm.toneConf[esm.regBus >> 1] += esm.dataBus;//overwrite
            break;
        }
        case 1: case 3: case 5: case 7: {
            esm.toneConf[(esm.regBus-1) >> 1] &= 0xff;//erase high byte
            esm.toneConf[(esm.regBus-1) >> 1] += ((short)esm.dataBus) << 8;//same
            break;
        }
        case 8: {
            esm.toneMask = esm.dataBus;
            break;
        }
        case 9: {
            esm.globVolume = esm.dataBus;
            break;
        }
        case 10: {
            esm.sampleConf = (esm.sampleConf & 0xff00);
            esm.sampleConf += esm.dataBus;
            break;
        }
        case 11: {
            esm.sampleConf = (esm.sampleConf & 0xff);
            esm.sampleConf += (short)esm.dataBus << 8;
            break;
        }
        case 12: {
            esm.sampleStart = (esm.sampleStart & 0xff00);
            esm.sampleStart += esm.dataBus;
            break;
        }
        case 13: {
            esm.sampleStart = (esm.sampleStart & 0xff);
            esm.sampleStart += (short)esm.dataBus << 8;
            break;
        }
        case 14: {
            esm.sampleEnd = (esm.sampleEnd & 0xff00);
            esm.sampleEnd += esm.dataBus;
            break;
        }
        case 15: {
            esm.sampleEnd = (esm.sampleEnd & 0xff);
            esm.sampleEnd += (short)esm.dataBus << 8;
            break;
        }
        default: break;
    }
}

void ESM::sampleCount() {
    if ((esm.sampleConf & 1u) == 0) {
        esm.sampleAddressBus = 0;
        return;
    }
    esm.sampleAddressBus++;

    if ((esm.sampleAddressBus > esm.sampleEnd)) {
        esm.sampleAddressBus = esm.sampleStart;
    }
}

void ESM::tones() {
    uint16_t toneDiv;
    uint32_t _xor;
    esm.psgOut = 0;
    esm.toneOut = esm.toneOut & 0xf0; //reset count outputs but not div outputs
    for (uint8_t i=0; i<4; i++) {
        esm.toneCounter[i] &= 0xfff;
        toneDiv = (esm.toneConf[i] & 0xfff);
        if (esm.toneCounter[i] > toneDiv - 1u) {
            esm.toneCounter[i] = 0u;
            esm.toneOut ^= (1u << (i+4));//write divided
                if (i < 3) {
                    _xor = ((esm.lfsr[i] >> 25) ^ (esm.lfsr[i] >> 19) ^ (esm.lfsr[i] >> 11) ^ (esm.lfsr[i] >> 9)) & 1u;
                } else {
                    _xor = ((esm.lfsr[i] >> 4) ^ (esm.lfsr[i] >> 3) ^ (esm.lfsr[i] >> 1) ^ esm.lfsr[i]) & 1u;
                }
                esm.lfsr[i] = (esm.lfsr[i] >> 1) | (!_xor << 31);
                esm.toneOut |= (0b1 << i);
            
        } else {
            esm.toneCounter[i]+=1u;
        }
        if (((esm.toneMask >> (i+4)) & 1u) == 1u) { //noise mode
            esm.psgOut |= ((esm.lfsr[i] & 1u) << i);
        } else {                                    //divided mode
            esm.psgOut |= (((esm.toneOut >> (i + 4*((esm.toneMask >> i) & 1u))) & 1u) << i);
        }
    }
}

void ESM::mixAnalog() {
    esm.aOut = ((esm.psgOut >> 0) & 1u) + ((esm.psgOut >> 1) & 1u) + ((esm.psgOut >> 2) & 1u) + ((esm.psgOut >> 3) & 1u);
    esm.aOut *= ((float)esm.globVolume/255);
}

uint16_t ESM::getSampleAddrBus() {
    return esm.sampleAddressBus;
}

uint8_t ESM::getRegBus() {
    return esm.regBus;
}

uint8_t ESM::getDataBus() {
    return esm.dataBus;
}

uint16_t ESM::getToneConf(uint8_t ch) {
    return esm.toneConf[ch];
}

uint16_t ESM::getToneCounter(uint8_t ch) {
    return esm.toneCounter[ch];
}

uint8_t ESM::getToneOut() {
    return esm.toneOut;
}

uint8_t ESM::getPSGOut() {
    return esm.psgOut;
}

float ESM::getAnalogOut() {
    return esm.aOut;
}

uint32_t ESM::getPC() {
    return esm.pc;
}

uint32_t ESM::getLFSR(uint8_t n) {
    return esm.lfsr[n];
}

uint8_t ESM::getRandOut() {
    return (esm.lfsr[0] & 0xff);
}

uint8_t ESM::getRegStat(uint8_t reg) {
    return esm.regStat[reg];
}

void ESM::writeRegBus(uint8_t reg) {
    esm.regBus = reg;
}

void ESM::writeDataBus(uint8_t data) {
    esm.dataBus = data;
}

void ESM::reset() {
    esm.sampleAddressBus = 0u;
    esm.regBus = 0u;
    esm.dataBus = 0u;
    esm.sampleStart = 0u;
    esm.sampleEnd = 0u;
    esm.sampleConf = 0u;
    esm.toneCounter[0] = 0u;
    esm.toneCounter[1] = 0u;
    esm.toneCounter[2] = 0u;
    esm.toneCounter[3] = 0u;
    esm.lfsr[0] = 0u;
    esm.lfsr[1] = 0u;
    esm.lfsr[2] = 0u;
    esm.lfsr[3] = 0u;
    esm.globVolume = 0u;
    esm.toneConf[0] = 0u;
    esm.toneConf[1] = 0u;
    esm.toneConf[2] = 0u;
    esm.toneConf[3] = 0u;
    esm.toneOut = 0u;
    esm.toneMask = 0u;
    esm.psgMultiplex = 0u;
    esm.psgOut = 0u;
    esm.pc = 0u;
    ESM::writeDataBus(0);
    for (uint8_t i=0;i<16;i++){
        ESM::writeRegBus(i);
        ESM::decodeRegister();
    }
}

char ESM::tick() {
    esm.pc++;
    ESM::decodeRegister();
    ESM::sampleCount();
    ESM::tones();
    mixAnalog(); //only for debug emu. hw will use multiplexing
    return 0;
}

const char * getEmuVersion() {
    return EMU_VERSION;
}
