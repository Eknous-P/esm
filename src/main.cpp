#include "main.h"
#include "esmdebug.h"
#include "gui.h"

#include <cstdint>
#include <cstring>

int main(int argc, char** argv) {
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