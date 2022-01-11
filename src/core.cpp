#include "core.h"

Core::Core() {
    // Initialize memory
    Memory memory(mem_size);
    ptrMemory = &memory;

    // Set the PC & Registers to initially zero
    // PC = memory.get_PC_start();

    // Initial registers
    for (int i = 0; i < num_regs; i++) {
        RF[i] = 0;
    }

    // Set run flag
    run = true;
}

Core::~Core() {}