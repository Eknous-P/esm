/* ESM "core" emulator

only contains the necessary stuff
*/

struct ESMCore_Chan_LN { // long noise
    unsigned short pitch;
    unsigned char volume;
    bool divide, noise;

    unsigned int lfsr;

    unsigned char output;
};

struct ESMCore_Chan_SN { // short noise
    unsigned short pitch;
    unsigned char volume;
    bool divide, noise;

    unsigned char lfsr;

    unsigned char output;
};

struct ESMCore_Counter {
    unsigned short counter;

    unsigned short pitch;
    bool state;
    unsigned short countStart, countEnd;
};

struct ESMCore {
    struct ESMCore_Chan_LN LNChan[2];
    struct ESMCore_Chan_SN SNChan[2];
    struct ESMCore_Counter sampleCounter;

    unsigned int masterOut;
}