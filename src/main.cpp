#include "core.h"

int main(int argc, char **argv) {
    std::cout << "Running Simulator\n";

    // Get
    int num_cycles = 0;
    if (argc > 1) {
        num_cycles = std::atoi(argv[1]);
    }

    Core CPU;

    // Run the simulator for the given number of cycles
    if (num_cycles != 0) {
        for (int i = 0; i < num_cycles; i++) {
            // Cycle
            CPU.tick();
        }
    } else {
        while (true) CPU.tick();
    }
    return 0;
}