#include "core.h"

int main() {
    std::cout << "Running Simulator \n";


    Core CPU;

    int num_cycles = 10;
    for (int i = 0; i < num_cycles; i++) {
        // Cycle
        CPU.tick();
    }
    return 0;
}