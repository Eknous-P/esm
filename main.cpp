#include <cstdio>
#include "esm.cpp"

ESM esm;

int main() {
    esm.decodeAddress(0b1100,0b11111111);
    esm.decodeAddress(0b1101,0b11111111);
    esm.decodeAddress(0b1110, 0b11111111);
    esm.decodeAddress(0b1111, 0b11111111);
    unsigned short printbuf;
    unsigned short printdata;
    while (1) {
        esm.tick();
        printdata = esm.getSampleAddrBus();
        if (printbuf != printdata) {
            printf("%d \n", printdata);
        }
        printbuf = printdata;
    }
}