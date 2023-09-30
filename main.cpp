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
                printf("commands:\nh: this\nt: single tick\nr: set register\nd: set data\nw: write to esm\nn: run N ticks\nq: quit\n");
                break;
            }
            case 0x74: {//t
                esm.tick();
                printf("REG: %b | DAT: %b | PC: %d | SAD: %b | TNO: %b \n",esm.getRegBus()%16,esm.getDataBus(),esm.getPC(),esm.getSampleAddrBus(),esm.getToneOut());
                break;
            }
            case 0x72: {//r
                printf("Enter register value (hex):");
                scanf("%x", &reg);
                printf("REG: %x/%b\n", reg,reg);
                break;
            }
            case 0x64: {//d
                printf("Enter data (hex):");
                scanf("%x", &data);
                printf("DAT: %x/%b\n", data,data);
                break;
            }
            case 0x77: {//w
                esm.writeRegBus(reg);
                esm.writeDataBus(data);
                esm.decodeRegister();
                printf("REG: %b | DAT: %b\n", esm.getRegBus(), esm.getDataBus());
            }
        }
    }
}