#include <cstdint>
class ESM {
    public:
    struct esm{
            /*====IO====*/
            uint8_t dataBus;
            uint8_t regBus; //only 1st 4 bits are used
            uint16_t sampleAddressBus;

            uint8_t psgOut;

            /*====INTERNAL MEMORY (LATCHES)====*/
            uint32_t lfsr0, lfsr1; //channels 1 and 2
            uint8_t lfsr2, lfsr3; //channels 3 and 4

            uint8_t globVolume;

            uint16_t toneConf[4]; //bits 1-12: freq divide, bits 13-16: volume
            uint16_t sampleConf; // bit 1: sample playing, bits 2-16: sample pitch freq divide
            uint8_t toneMask; // bits 1-4: output divide, bits 5-8: noise mode

            uint16_t toneCounter[4]; //only 1st 12 bits
            uint8_t toneOut; //bits 1-4: tone counter reset 1-4, bits 5-8, tone out 1-4 (with dividers)

            uint16_t sampleStart, sampleEnd;

            uint8_t psgMultiplex; //1st 2 bits

            uint32_t pc;//prog counter?

    } esm;

    void writeRegBus(uint8_t reg);
    void writeDataBus(uint8_t data);

    void decodeRegister();
    void sampleCount();
    void longLFSR();
    void shortLFSR();
    void tones();
    void tick();

    void reset();

    uint16_t getSampleAddrBus();
    uint8_t getRegBus();
    uint8_t getDataBus();
    uint8_t getToneConf(uint8_t ch);
    uint8_t getToneCounter(uint8_t ch);
    uint8_t getToneOut();
    uint32_t getPC();
    uint8_t getRandOut();
};