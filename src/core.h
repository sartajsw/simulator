#include <iostream>
#include "memory.h"
#include "instructions.h"

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

    // Program Counter
    unsigned int PC = 0;

    // Registers
    unsigned int RF[num_regs] = {0};

    // Memory interface
    Memory *ptrMemory;

    // Instructions
    Instruction instr_type;
    unsigned int instruction;
    InstructionFormat instr_format;

    // Fields
    unsigned int funct7; 
    unsigned int rs2;
    unsigned int rs1;
    unsigned int funct3;
    unsigned int rd;
    unsigned int opcode;
    unsigned int immidiate;
    unsigned int decode_bits;

    bool funct7_valid;
    bool rs2_valid;
    bool rs1_valid;
    bool funct3_valid;
    bool rd_valid;
    bool imm_valid;

    // ALU
    unsigned int sltu_rslt;
    unsigned int sltiu_rslt;
    unsigned long sext_src1;
    unsigned long sra_result;
    unsigned long srai_result;

    unsigned int result;

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

    // Get status of run flag
    bool get_run();

    // Increment PC
    void increment_PC(unsigned int offset);
};
