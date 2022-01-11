#include "core.h"

Core::Core() {
    // Initialize memory
    Memory memory(mem_size);
    ptrMemory = &memory;

    // Load program into memory
    ptrMemory->load_program();

    // Set the PC & Registers to initially zero
    PC = ptrMemory->get_start();

    // Initial registers
    for (int i = 0; i < num_regs; i++) {
        RF[i] = 0;
    }

    // Set run flag
    run = true;
}

Core::~Core() {}

void Core::tick() {
    // If run flag is set
    if (run == true) {
        // Process instruction
        process_instruction();
    }

    // Increment intruction count
    instr_count++;
}

void Core::process_instruction() {}