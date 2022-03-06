#include <iostream>
#include "memory.h"
// #include "instructions.h"
// #include "pipe.h"

// Forward Declarations
class FetchStage;
class DecodeStage;
class ExecuteStage;
class MemoryAccessStage;
class WritebackStage;

class Core {
private:
    // Instruction count
    int instr_count = 0;

    // Size of the memory
    static const int mem_size = 0x00100000;
    // Number of registers
    static const int num_regs = 32;

    // Run flag
    bool run = false;

public:
    // Constructor
    Core();
    // Distructor
    ~Core();

    // Pipeline Stages
    FetchStage *Fetch;
    DecodeStage *Decode;
    ExecuteStage *Execute;
    MemoryAccessStage *MemoryAccess;
    WritebackStage *Writeback;

    // Program Counter
    unsigned int PC = 0;

    // Registers
    unsigned int RF[num_regs] = {0};

    // Memory interface
    Memory *ptrMemory;

    // Execute a cycle 
    void tick();

    // Get status of run flag
    bool get_run();

    // Print the Register File
    void print_RF();

    // Increment PC
    void increment_PC();

    // Read from memory
    unsigned int read_PC();

    // Flush pileline
    void flush(int stage);
};
