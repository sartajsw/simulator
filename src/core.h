#include <iostream>
#include "memory.h"

class Core {
private:
    // Size of the memory
    static const int mem_size = 0x00100000;
    // Number of registers
    static const int num_regs = 32;

    // Run flag
    bool run = false;

    // Program Counter
    uint32_t PC = 0;

    // Registers
    uint32_t RF[num_regs] = {0};

    // Memory interface
    Memory *ptrMemory;
public:
    // Constructor
    Core();
    // Distructor
    ~Core();
    
    // Execute a cycle 
    void tick();

    // Process the next instruction 
    // Perform all the steps of the processor in a non-pipelined fashion.
    void process_instruction();
};