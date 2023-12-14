#include "main.h"
#include "esmdebug.h"

std::string toBin(uint32_t n) { // i hate that ive had to do this
    std::string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    return r;
}


int main(int argc, char** argv) {
    ESM esm;
    uint8_t reg = 0, dat = 0;

    switch (argc) {
        case 2: { // yes argument
            if (argv[1] == "-cli") {
                return run_cli();
            }
            return 1;
        }
        default: {
            return start_gui();
        }
    }
}