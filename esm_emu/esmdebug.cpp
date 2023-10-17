#include <cstdint>
#include "esmdebug.h"

#define EMU_VERSION "v0.2"

/*====REGISTERS (once more)====

ADDR|NAME======================|DATA====
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
            esm.toneConf[esm.regBus >> 1] &= 0b1111111100000000;//erase low byte
            esm.toneConf[esm.regBus >> 1] += esm.dataBus;//overwrite
            break;
        }
        case 1: case 3: case 5: case 7: {
            esm.toneConf[(esm.regBus-1) >> 1] &= 0b0000000011111111;//erase high byte
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
            esm.sampleConf = (esm.sampleConf & 0b1111111100000000);
            esm.sampleConf += esm.dataBus;
            break;
        }
        case 11: {
            esm.sampleConf = (esm.sampleConf & 0b0000000011111111);
            esm.sampleConf += (short)esm.dataBus*256;
            break;
        }
        case 12: {
            esm.sampleStart = (esm.sampleStart & 0b1111111100000000);
            esm.sampleStart += esm.dataBus;
            break;
        }
        case 13: {
            esm.sampleStart = (esm.sampleStart & 0b0000000011111111);
            esm.sampleStart += (short)esm.dataBus*256;
            break;
        }
        case 14: {
            esm.sampleEnd = (esm.sampleEnd & 0b1111111100000000);
            esm.sampleEnd += esm.dataBus;
            break;
        }
        case 15: {
            esm.sampleEnd = (esm.sampleEnd & 0b0000000011111111);
            esm.sampleEnd += (short)esm.dataBus*256;
            break;
        }
        default: break;
    }
}

void ESM::sampleCount() {
    esm.sampleAddressBus++;

    if ((esm.sampleAddressBus > esm.sampleEnd) && ((esm.sampleConf & 0b1u) == 0u)) {
        esm.sampleAddressBus = esm.sampleStart;
    }
    else if ((esm.sampleConf << 4) > 0) {
        esm.sampleAddressBus = 0;
    }
}

void ESM::tones() {
    uint16_t toneDiv[4];
    esm.toneOut = esm.toneOut & 0b11110000; //reset count outputs but not div outputs
    for (uint8_t i=0; i<4; i++) {
        esm.toneCounter[i] &= 0b0000111111111111;
        toneDiv[i] = (esm.toneConf[i] & 0b0000111111111111);
        if (esm.toneCounter[i] > toneDiv[i] - 1u) {
            esm.toneCounter[i] = 0u;
            esm.toneOut ^= (1u << (i+4));//write divided
            switch (i) {//lfsrs and undivided (pulse)
                case 0: {
                    uint32_t xor0 = ((esm.lfsr0 >> 25) ^ (esm.lfsr0 >> 19) ^ (esm.lfsr0 >> 11) ^ (esm.lfsr0 >> 9)) & 1u;
                    esm.lfsr0 = (esm.lfsr0 >> 1) | (!xor0 << 31);
                    esm.toneOut |= 0b1;
                    break;
                }
                case 1: {
                    uint32_t xor1 = ((esm.lfsr1 >> 25) ^ (esm.lfsr1 >> 19) ^ (esm.lfsr1 >> 11) ^ (esm.lfsr1 >> 9)) & 1u;
                    esm.lfsr1 = (esm.lfsr1 >> 1) | (!xor1 << 31);
                    esm.toneOut |= 0b10;
                    break;
                }
                case 2: {
                    uint8_t xor2 = ((esm.lfsr2 >> 4) ^ (esm.lfsr2 >> 3) ^ (esm.lfsr2 >> 1) ^ esm.lfsr2) & 1u;
                    esm.lfsr2 = (esm.lfsr2 >> 1) | (!xor2 << 7);
                    esm.toneOut |= 0b100;
                    break;
                }
                case 3: {
                    uint8_t xor3 = ((esm.lfsr3 >> 4) ^ (esm.lfsr3 >> 3) ^ (esm.lfsr3 >> 1) ^ esm.lfsr3) & 1u;
                    esm.lfsr3 = (esm.lfsr3 >> 1) | (!xor3 << 7);
                    esm.toneOut |= 0b1000;
                    break;
                }
                default: break;
            }
        } else {
            esm.toneCounter[i]+=1u;
        }
    }
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

uint8_t ESM::getToneCounter(uint8_t ch) {
    return esm.toneCounter[ch];
}

uint8_t ESM::getToneOut() {
    return esm.toneOut;
}

uint32_t ESM::getPC() {
    return esm.pc;
}

uint32_t ESM::getLFSR(uint8_t n) {
    switch (n) {
        case 0: return esm.lfsr0; break;
        case 1: return esm.lfsr1; break;
        case 2: return esm.lfsr2; break;
        case 3: return esm.lfsr3; break;
        default: return 0xffff; break; //wrong number
    }
}

uint8_t ESM::getRandOut() {
    return (esm.lfsr0 & 0xff);
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
    esm.lfsr0 = 0u;
    esm.lfsr1 = 0u;
    esm.lfsr2 = 0u;
    esm.lfsr3 = 0u;
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
    return 0;
}

const char * getEmuVersion() {
    return EMU_VERSION;
}