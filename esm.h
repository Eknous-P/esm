class ESM {
    public:
    struct esm{
            /*====IO====*/
            unsigned char dataBus;
            unsigned char regBus; //only 1st 4 bits are used
            unsigned short sampleAddressBus;

            unsigned char psgOut;

            /*====INTERNAL MEMORY (LATCHES)====*/
            unsigned int lfsr0, lfsr1; //channels 1 and 2
            unsigned char lfsr2, lfsr3; //channels 3 and 4

            unsigned char globVolume;

            unsigned short toneConf[4]; //bits 1-12: freq divide, bits 13-16: volume
            unsigned short sampleConf; // bit 1: sample playing, bits 2-16: sample pitch freq divide
            unsigned char toneMask; // bits 1-4: output divide, bits 5-8: noise mode

            unsigned short toneCounter[4]; //only 1st 12 bits
            unsigned char toneOut; //bits 1-4: tone counter reset 1-4, bits 5-8, tone out 1-4 (with dividers)

            unsigned short sampleStart, sampleEnd;

            unsigned char psgMultiplex; //1st 2 bits

            unsigned int pc;//prog counter?

    } esm;

    void writeRegBus(unsigned char reg);
    void writeDataBus(unsigned char data);

    void decodeRegister();
    void sampleCount();
    void longLFSR();
    void shortLFSR();
    void tones();
    void tick();

    void reset();

    unsigned short getSampleAddrBus();
    unsigned char getRegBus();
    unsigned char getDataBus();
    unsigned char getToneOut();
    unsigned int getPC();
    unsigned char getRandOut();
};