#include <stdio.h>
#include "esm.h"

ESM esm;

unsigned int iter=0;
unsigned int data;
unsigned int reg;
char cmd[80];

int main(int argc, char* argv[]) {
    esm.reset();
    printf("ESM test tool v0.1\n");
    while (1) {
        printf("Enter command (h for help):");
        scanf("%79s", cmd);
        switch (cmd[0]) {
            case 0x71: {//q
                printf("quitting\n");
                return 0;
            }
            case 0x68: {//h
                printf("Commands (case sensitive):\n  h: this\n  t: single tick\n  r: set register\n  d: set data\n  w: write to esm\n  n: run N ticks\n  R: reset chip\n  q: quit\n");
                break;
            }
            case 0x74: {//t
                esm.tick();
                printf("REG: %b | DAT: %b | PC: %d | SAD: %b | TCF: %b/%b/%b/%b | TCN: %b/%b/%b/%b | TNO: %b | RNG: %b \n",esm.getRegBus()%16,esm.getDataBus(),esm.getPC(),esm.getSampleAddrBus(),esm.getToneConf(3),esm.getToneConf(2),esm.getToneConf(1),esm.getToneConf(0),esm.getToneCounter(3),esm.getToneCounter(2),esm.getToneCounter(1),esm.getToneCounter(0),esm.getToneOut(),esm.getRandOut());
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
                break;
            }
            case 0x6e :{//n
                printf("Enter iterations amount (dec):");
                scanf("%d",&iter);
                for (unsigned int i = 0; i < iter; i++) {
                    esm.tick();
                    printf("REG: %b | DAT: %b | PC: %d | SAD: %b | TCF: %b/%b/%b/%b | TCN: %b/%b/%b/%b | TNO: %b | RNG: %b \n",esm.getRegBus()%16,esm.getDataBus(),esm.getPC(),esm.getSampleAddrBus(),esm.getToneConf(3u),esm.getToneConf(2u),esm.getToneConf(1u),esm.getToneCounter(0u),esm.getToneCounter(3u),esm.getToneCounter(2u),esm.getToneCounter(1u),esm.getToneConf(0u),esm.getToneOut(),esm.getRandOut());
                }
                break;
            }
            case 0x52: {//R
                esm.reset();
                printf("REG: %b | DAT: %b | PC: %d | SAD: %b | TCF: %b/%b/%b/%b | TCN: %b/%b/%b/%b | TNO: %b | RNG: %b \n",esm.getRegBus()%16,esm.getDataBus(),esm.getPC(),esm.getSampleAddrBus(),esm.getToneConf(3u),esm.getToneConf(2u),esm.getToneConf(1u),esm.getToneCounter(0u),esm.getToneCounter(3u),esm.getToneCounter(2u),esm.getToneCounter(1u),esm.getToneConf(0u),esm.getToneOut(),esm.getRandOut());
                break;
            }
            default: {
                printf("Invalid comand!\n");
                break;
            }
        }
    }
}