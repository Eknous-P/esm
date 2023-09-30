#include <stdio.h>
#include "esm.h"

ESM esm;

unsigned int iter=0;
unsigned int data;
unsigned int reg;
char cmd[80];

int main(int argc, char* argv[]) {
    esm.reset();
    printf("ESM test tool v0\n");
    while (1) {
        printf("Enter command (h for help):");
        scanf("%79s", cmd);
        switch (cmd[0]) {
            case 0x71: {//q
                printf("quitting\n");
                return 0;
            }
            case 0x68: {//h
                printf("commands:\nh: this\nt: single tick\nr: write register\nd: write data\nn: run N ticks\nq: quit\n");
                break;
            }
            case 0x74: {//t
                esm.tick();
                printf("REG: %b | DAT: %b | PC: %d | SAD: %b | TNO: %b \n",esm.getRegBus()%16,esm.getDataBus(),esm.getPC(),esm.getSampleAddrBus(),esm.getToneOut());
                break;
            }
            case 0x72 :{//r
                printf("Enter register value (binary):");
                scanf("%b", &reg);
                esm.decodeRegister((char)reg, data);
                printf("REG: %b ", esm.getRegBus());
                break;
            }

        }
    }
}