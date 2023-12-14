#include <stdio.h>
#include "main.h"


unsigned int iter=0;
char cmd[80];

int run_cli() {
//     esm.reset();
//     printf("ESM test tool v0.1\n");
//     while (1) {
//         printf("Enter command (h for help):");
//         scanf("%79s", cmd);
//         switch (cmd[0]) {
//             case 0x71: {//q
//                 printf("quitting\n");
//                 return 0;
//             }
//             case 0x68: {//h
//                 printf("Commands (case sensitive):\n  h: this\n  t: single tick\n  r: set register\n  d: set data\n  w: write to esm\n  n: run N ticks\n  s: register status\n  R: reset chip\n  q: quit\n");
//                 break;
//             }
//             case 0x74: {//t
//                 esm.tick();
//                 printf("REG: %s | DAT: %s | PC: %d | SAD: %s | TCF: %s/%s/%s/%s | TCN: %s/%s/%s/%s | TNO: %s | RNG: %s \n",esm.getRegBus()%16,esm.getDataBus(),esm.getPC(),esm.getSampleAddrBus(),esm.getToneConf(3),esm.getToneConf(2),esm.getToneConf(1),esm.getToneConf(0),esm.getToneCounter(3),esm.getToneCounter(2),esm.getToneCounter(1),esm.getToneCounter(0),esm.getToneOut(),esm.getRandOut());
//                 break;
//             }
//             case 0x72: {//r
//                 printf("Enter register value (hex):");
//                 scanf("%x", &reg);
//                 printf("REG: %x/%s\n", reg,reg);
//                 break;
//             }
//             case 0x64: {//d
//                 printf("Enter data (hex):");
//                 scanf("%x", &dat);
//                 printf("DAT: %x/%s\n", dat,dat);
//                 break;
//             }
//             case 0x77: {//w
//                 esm.writeRegBus(reg);
//                 esm.writeDataBus(dat);
//                 esm.decodeRegister();
//                 printf("REG: %s | DAT: %s\n", esm.getRegBus(), esm.getDataBus());
//                 break;
//             }
//             case 0x6e: {//n
//                 printf("Enter iterations amount (dec):");
//                 scanf("%d",&iter);
//                 for (unsigned int i = 0; i < iter; i++) {
//                     esm.tick();
//                     printf("REG: %s | DAT: %s | PC: %d | SAD: %s | TCF: %s/%s/%s/%s | TCN: %s/%s/%s/%s | TNO: %s | RNG: %s \n",esm.getRegBus()%16,esm.getDataBus(),esm.getPC(),esm.getSampleAddrBus(),esm.getToneConf(3),esm.getToneConf(2),esm.getToneConf(1),esm.getToneConf(0),esm.getToneCounter(3),esm.getToneCounter(2),esm.getToneCounter(1),esm.getToneCounter(0),esm.getToneOut(),esm.getRandOut());
//                 }
//                 break;
//             }
//             case 0x73: {//s
//                 printf("Regitsers...\n");
//                 for(unsigned char i=0; i<16; i++) {
//                     printf("REG: %x | DAT: %s\n",i, esm.getRegStat(i));
//                 }
//                 break;
//             }
//             case 0x52: {//R
//                 esm.reset();
//                 printf("REG: %s | DAT: %s | PC: %d | SAD: %s | TCF: %s/%s/%s/%s | TCN: %s/%s/%s/%s | TNO: %s | RNG: %s \n",esm.getRegBus()%16,esm.getDataBus(),esm.getPC(),esm.getSampleAddrBus(),esm.getToneConf(3),esm.getToneConf(2),esm.getToneConf(1),esm.getToneConf(0),esm.getToneCounter(3),esm.getToneCounter(2),esm.getToneCounter(1),esm.getToneCounter(0),esm.getToneOut(),esm.getRandOut());
//                 break;
//             }
//             default: {
//                 printf("Invalid comand!\n");
//                 break;
//             }
//         }
    // }
    return 0;
}