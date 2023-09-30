#include <cstdint>
#include "esm.h"

/*====REGISTERS (once more)====

ADDR|NAME=====================|DATA====
----|-------------------------|--------
0000|TONE 1 CONFIG 1 =========|FFFFFFFF
0001|TONE 1 CONFIG 2 =========|VVVVFFFF
0010|TONE 2 CONFIG 1 =========|FFFFFFFF
0011|TONE 2 CONFIG 2 =========|VVVVFFFF
0100|TONE 3 CONFIG 1 =========|FFFFFFFF
0101|TONE 3 CONFIG 2 =========|VVVVFFFF
0110|TONE 4 CONFIG 1 =========|FFFFFFFF
0111|TONE 4 CONFIG 2 =========|VVVVFFFF
1000|GLOBAL TONE GONFIG (MASK)|NNNNDDDD
1001|GLOBAL VOLUME ===========|VVVVVVVV *
1010|SAMPLE CONFIG 1 =========|FFFFFFFF
1011|SAMPLE CONFIG 2 =========|VVVVFFFF
1100|SAMPLE LOOP START LOW ===|AAAAAAAA
1101|SAMPLE LOOP START HIGH ==|AAAAAAAA
1110|SAMPLE LOOP END LOW =====|AAAAAAAA
1111|SAMPLE LOOP END HIGH ====|AAAAAAAA
---------------------------------------
(F)requency    (D)ivider
(V)olume       (A)ddress
(N)oise

*/

void ESM::decodeRegister(unsigned char reg, unsigned char data) {
    switch (reg % 16) {
        case 0: case 2: case 4: case 6: {
            esm.toneConf[reg] = (esm.toneConf[reg] & 0b1111111100000000);//erase low byte
            esm.toneConf[reg] += data;//overwrite
            break;
        }
        case 1: case 3 :case 5: case 7:{
            esm.toneConf[reg] = (esm.toneConf[reg] & 0b0000000011111111);//erase high byte
            esm.toneConf[reg] += data*256;
            break;
        }
        case 8: {
            esm.toneMask = data;
            break;
        }
        case 9: {
            esm.globVolume = data;
            break;
        }
        case 10: {
            esm.sampleConf = (esm.sampleConf & 0b1111111100000000);
            esm.sampleConf += data;
            break;
        }
        case 11: {
            esm.sampleConf = (esm.sampleConf & 0b0000000011111111);
            esm.sampleConf += data*256;
            break;
        }
        case 12: {
            esm.sampleStart = (esm.sampleStart & 0b1111111100000000);
            esm.sampleStart += data;
            break;
        }
        case 13: {
            esm.sampleStart = (esm.sampleStart & 0b0000000011111111);
            esm.sampleStart += data*256;
            break;
        }
        case 14: {
            esm.sampleEnd = (esm.sampleEnd & 0b1111111100000000);
            esm.sampleEnd += data;
            break;
        }
        case 15: {
            esm.sampleEnd = (esm.sampleEnd & 0b0000000011111111);
            esm.sampleEnd += data*256;
            break;
        }
        default: break;
    }
}

void ESM::sampleCount() {
    esm.sampleAddressBus++;

    if ((esm.sampleAddressBus > esm.sampleEnd) && ((esm.sampleConf << 4) == 0)) {
        esm.sampleAddressBus = esm.sampleStart;
    }
    else if ((esm.sampleConf << 4) > 0) {
        esm.sampleAddressBus = 0;
    }
}

void ESM::longLFSR() {
    uint32_t xor0 = ((esm.lfsr0 >> 25) ^ (esm.lfsr0 >> 19) ^ (esm.lfsr0 >> 11) ^ (esm.lfsr0 >> 9)) & 1u;
    uint32_t xor1 = ((esm.lfsr1 >> 25) ^ (esm.lfsr1 >> 19) ^ (esm.lfsr1 >> 11) ^ (esm.lfsr1 >> 9)) & 1u;
    //shift (why did i write shirt before???????)
    esm.lfsr0 = (esm.lfsr0 >> 1) | (!xor0 << 31);
    esm.lfsr1 = (esm.lfsr1 >> 1) | (!xor1 << 31);
}

void ESM::shortLFSR() {
    unsigned char xor2 = ((esm.lfsr2 >> 4) ^ (esm.lfsr2 >> 3) ^ (esm.lfsr2 >> 1) ^ esm.lfsr2) & 1u;
    unsigned char xor3 = ((esm.lfsr3 >> 4) ^ (esm.lfsr3 >> 3) ^ (esm.lfsr3 >> 1) ^ esm.lfsr3) & 1u;
    esm.lfsr2 = (esm.lfsr2 >> 1) | (!xor2 << 7);
    esm.lfsr3 = (esm.lfsr3 >> 1) | (!xor3 << 7);
}

void ESM::tones() {
    unsigned short toneFreq;
    unsigned char toneOutBuf;
    for (unsigned char i=0; i<4; i++) {
        esm.toneCounter[i]++;
        esm.toneOut = esm.toneOut & 0b11110000; //reset count outputs but not div outputs
        toneFreq = esm.toneConf[i] & 0b0000111111111111;
        if (esm.toneCounter[i] == toneFreq) {
            esm.toneOut += 1u << i;
            toneOutBuf = esm.toneOut;
            // toneOut
        }
    }
}

unsigned short ESM::getSampleAddrBus() {
    return esm.sampleAddressBus;
}

unsigned char ESM::getRegBus() {
    return esm.regBus;
}

unsigned char ESM::getDataBus() {
    return esm.dataBus;
}

unsigned char ESM::getToneOut() {
    return esm.toneOut;
}

unsigned char ESM::getPC() {
    return esm.pc;
}

unsigned char ESM::getRandOut() {
    return (esm.lfsr0 & 0xff);
}

void ESM::reset() {
    esm.sampleAddressBus = 0;
    esm.regBus = 0;
    esm.dataBus = 0;
    esm.sampleStart = 0;
    esm.sampleEnd = 0;
    esm.sampleConf = 0;
    esm.toneCounter[0] = 0;
    esm.toneCounter[1] = 0;
    esm.toneCounter[2] = 0;
    esm.toneCounter[3] = 0;
    esm.lfsr0 = 0;
    esm.lfsr1 = 0;
    esm.lfsr2 = 0;
    esm.lfsr3 = 0;
    esm.globVolume = 0;
    esm.toneConf[0] = 0;
    esm.toneConf[1] = 0;
    esm.toneConf[2] = 0;
    esm.toneConf[3] = 0;
    esm.toneOut = 0;
    esm.toneMask = 0;
    esm.psgMultiplex = 0;
    esm.psgOut = 0;
    esm.pc = 0;
}

void ESM::tick() {
    esm.pc++;
    ESM::decodeRegister(esm.regBus, esm.dataBus);
    ESM::longLFSR();
    ESM::shortLFSR();
    ESM::sampleCount();
    ESM::tones();
}
