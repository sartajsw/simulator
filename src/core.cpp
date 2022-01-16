#include "core.h"

Core::Core() {
    std::cout << "Core Constructor \n";

    // Initialize memory
    Memory memory(mem_size);
    this->ptrMemory = &memory;

    // Load program into memory
    this->ptrMemory->load_hex();

    // Set the PC & Registers to initially zero
    PC = this->ptrMemory->get_start();

    // Initial registers
    for (int i = 0; i < num_regs; i++) {
        this->RF[i] = 0;
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

void Core::process_instruction() {
    /* Fetch */
    this->instruction = this->ptrMemory->read(this->PC);
    std::cout << "Instruction: " << this->instruction << std::endl;

    /* Decode */
    // Get instruction format
    unsigned int masked_instr = (this->instruction & (0x1F << 2)) >> 2;
    
    std::cout << "Instruction Format: ";
    InstructionFormat instr_format = InstructionFormat::Invalid;
    switch (masked_instr) {
        case 0b01011:
        case 0b10100:
        case 0b01100:
        case 0b01110:
            instr_format = InstructionFormat::R;
            std::cout << "R\n";
            break;
        case 0b00000:
        case 0b00001:
        case 0b00100:
        case 0b00110:
        case 0b11001:
            instr_format = InstructionFormat::I;
            std::cout << "I\n";
            break;
        case 0b01000:
        case 0b01001:
            instr_format = InstructionFormat::S;
            std::cout << "S\n";
            break;
        case 0b11000:
            instr_format = InstructionFormat::B;
            std::cout << "B\n";
            break;
        case 0b00101:
        case 0b01101:
            instr_format = InstructionFormat::U;
            std::cout << "U\n";
            break;
        case 0b11011:
            instr_format = InstructionFormat::J;
            std::cout << "J\n";
            break;
        default:
            std::cerr << "Instruction format invalid! \n";
            exit(0);
    }

    // Extract fields
    this->funct7  = (this->instruction & 0b1111111) << 25;
    this->rs2 = (this->instruction & 0b11111) << 20;
    this->rs1 = (this->instruction & 0b11111) << 15;
    this->funct3 = (this->instruction & 0b111) << 12;
    this->rd = (this->instruction & 0b11111) << 7;
    this->opcode = (this->instruction & 0b1111111) << 0;

    // Extract immidiate value
    switch (instr_format) {
        case InstructionFormat::I:
            this->immidiate = ((0x1FFFFF && (this->instruction & (0x8 << 28))) << 11) | 
                              ((this->instruction >> 20) & 0x7FF);
            break;
        case InstructionFormat::S:
            this->immidiate = ((0x1FFFFF && (this->instruction & (0x8 << 28))) << 11) | 
                              (((this->instruction >> 25) & 0x3F) << 5) | ((this->instruction >> 7) & 0x1F); 
            break;
        case InstructionFormat::B:
            this->immidiate = ((0xFFFFF && (this->instruction & (0x8 << 28))) << 12) | 
                              (((this->instruction >> 7) & 0x1) << 11) | 
                              (((this->instruction >> 25) & 0x3F) << 5) | 
                              (((this->instruction >> 7) & 0xF) << 1) | 0;
            break;
        case InstructionFormat::U:
            this->immidiate = (((this->instruction >> 20) && 0xFFFFF) << 12) | 0;
            break;
        case InstructionFormat::J:
            this->immidiate = (0xFFF && (this->instruction & (0x8 << 28))) | 
                              (((this->instruction >> 12) & 0xFF) << 12) | 
                              (((this->instruction >> 20) & 0x1) << 11) | 
                              (((this->instruction >> 21) & 0x3FF) << 1) | 0;
            break;
        default:
            this->immidiate = 0;
    }

    // Check field validity
    this->funct7_valid = (instr_format == InstructionFormat::R);
    this->rs2_valid = (instr_format == InstructionFormat::R) || 
                      (instr_format == InstructionFormat::S) || 
                      (instr_format == InstructionFormat::B);
    this->rs1_valid = (instr_format == InstructionFormat::R) || 
                      (instr_format == InstructionFormat::S) || 
                      (instr_format == InstructionFormat::I) || 
                      (instr_format == InstructionFormat::B);
    this->funct3_valid = (instr_format == InstructionFormat::R) || 
                         (instr_format == InstructionFormat::S) || 
                         (instr_format == InstructionFormat::I) || 
                         (instr_format == InstructionFormat::B);
    this->rd_valid = ((instr_format == InstructionFormat::R) || 
                      (instr_format == InstructionFormat::I) || 
                      (instr_format == InstructionFormat::U) || 
                      (instr_format == InstructionFormat::J)) && 
                     (this->rd != 0);
    this->imm_valid = !(instr_format == InstructionFormat::R);

    std::cout << "Extracted fields:\n";
    std::cout << "Funct7 - " << this->funct7 << "\n"; 
    std::cout << "Rs2 - " << this->rs2 << "\n";
    std::cout << "Rs1 - " << this->rs1 << "\n";
    std::cout << "Funct3 - " << this->funct3 << "\n";
    std::cout << "Rd - " << this->rd << "\n";
    std::cout << "Opcode - " << this->opcode << "\n";
    std::cout << "Immidiate - " << this->immidiate << "\n";

    // Extract the decode bits needed
    unsigned int decode_bits = (((this->instruction & (0x4 << 28)) << 10) | 
                               (this->funct3 << 7) | 
                               (this->opcode)) & 0x7FF;

    /* Execute */


    /* Memory */

    /* Writeback */


    // Increment PC
    this->increment_PC(4);

    std::cout << std::endl;
}

bool Core::get_run() {
    return this->run;
}

void Core::increment_PC(unsigned int offset = 4) {
    this->PC += offset;
}
