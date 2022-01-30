#include <iostream>
#include "instructions.h"

class PipelineStage {
public:
    void tick() { std::cout << "Tick pipeline stage\n"; }
    void read() { std::cout << "Read from previous pipeline stage\n"; }
    void write() { std::cout << "Write to the stage's latch\n"; }
};

class FetchStage: public PipelineStage {
private:
    // Core interface
    Core *ptrCore;

    // Instruction 
    unsigned int instruction;

public:
    // Constructor
    FetchStage(Core *cpu_core);
    // Distructor
    ~FetchStage();

    // Latch for next stage
    unsigned int instructionLatch;

    // Execute a cycle
    void tick();

    // Read instructions from memory
    void read();

    // Write to latch
    void write();
};

class DecodeStage: public PipelineStage {
private:
    // Core Interface
    Core *ptrCore;

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

    unsigned int src1_value;
    unsigned int src2_value;

public:
    // Constructor
    DecodeStage(Core *cpu_core);
    // Distructor
    ~DecodeStage();

    // Latch for next stage
    bool rd_validLatch;
    Instruction instr_typeLatch;
    InstructionFormat instr_formatLatch;
    unsigned int immidiateLatch;
    unsigned int src1_valueLatch;
    unsigned int src2_valueLatch;

    // Execute a cycle
    void tick();

    // Read from prev stage
    void read();

    // Write to latch
    void write();
};

class ExecuteStage: public PipelineStage {
private:
    // Core interface
    Core *ptrCore;

    // ALU
    unsigned int sltu_rslt;
    unsigned int sltiu_rslt;
    unsigned long sext_src1;
    unsigned long sra_result;
    unsigned long srai_result;

    unsigned int result;
    
    // PC / Branch Logic
    bool taken_branch;
public:
    // Constructor
    ExecuteStage(Core *cpu_core);
    // Distructor
    ~ExecuteStage();

    // Latch for the next stage
    bool rd_validLatch;
    unsigned int resultLatch;
    InstructionFormat instr_formatLatch;
    unsigned int src2_valueLatch;

    // Execute a cycle
    void tick();

    // Read from prev stage
    void read();

    // Write to latch
    void write();
};

class MemoryAccessStage: public PipelineStage {
private:
    // Core interface
    Core *ptrCore;

    bool rd_valid;
    unsigned int result;
    InstructionFormat instr_format;

public:
    // Constructor
    MemoryAccessStage(Core *cpu_core);
    // Distructor
    ~MemoryAccessStage();

    // Latch for the next stage
    bool rd_validLatch;
    unsigned int resultLatch;

    // Execute a cycle
    void tick();

    // Read from prev stage
    void read();

    // Write to latch
    void write();
};

class WritebackStage: public PipelineStage {
private:
    // Core interface
    Core *ptrCore;

    bool rd_valid;
    unsigned int result;

public:
    // Constructor
    WritebackStage(Core *cpu_core);
    // Distructor
    ~WritebackStage();

    // Execute a cycle
    void tick();

    // Read from prev stage
    void read();
};