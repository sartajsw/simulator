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
    unsigned int instruction = this->ptrMemory->read(this->PC);
    std::cout << "Instruction: " << instruction << std::endl;

    /* Decode */
    // Get instruction type
    unsigned int masked_instr = (instruction & (0x1F << 2)) >> 2;
    
    if ((masked_instr == 0b01011) || 
        (masked_instr == 0b10100) || 
        (masked_instr == 0b01100) || 
        (masked_instr == 0b01110)) {
        this->is_r_instr = true;
    } else {
        this->is_r_instr = false;
    }

    if ((masked_instr == 0b00000) || 
        (masked_instr == 0b00001) ||
        (masked_instr == 0b00100) ||
        (masked_instr == 0b00110) ||
        (masked_instr == 0b11001)) {
        this->is_i_instr = true;
    } else {
        this->is_i_instr = false;
    }

    if ((masked_instr == 0b01000) ||
        (masked_instr == 0b01001)) {
        this->is_s_instr = true;
    } else {
        this->is_s_instr = false;
    }
    
    if (masked_instr == 0b11000) {
        this->is_b_instr = true;
    } else {
        this->is_b_instr = false;
    }

    if ((masked_instr == 0b00101) ||
        (masked_instr == 0b01101)) {
        this->is_u_instr = true;
    } else {
        this->is_u_instr = false;
    }
    
    if  (masked_instr == 0b11011) {
        this->is_j_instr = true;
    } else {
        this->is_j_instr = false;
    }

    // DEBUG - printing instruction type
    std::cout << "Instruction Type: ";
    if (this->is_r_instr == true) {
        std::cout << "R\n";
    }
    if (this->is_i_instr == true) {
        std::cout << "I\n";
    }
    if (this->is_s_instr == true) {
        std::cout << "S\n";
    }
    if (this->is_b_instr == true) {
        std::cout << "B\n";
    }
    if (this->is_u_instr == true) {
        std::cout << "U\n";
    }
    if (this->is_j_instr == true) {
        std::cout << "J\n";
    }

    // Extract fields
    this->funct7  = (instruction & 0b1111111) << 25;
    this->rs2 = (instruction & 0b11111) << 20;
    this->rs1 = (instruction & 0b11111) << 15;
    this->funct3 = (instruction & 0b111) << 12;
    this->rd = (instruction & 0b11111) << 7;
    this->opcode = (instruction & 0b1111111) << 0;

    // Extract immidiate value
    if (this->is_i_instr == true) {
        this->immidiate = ((0x1FFFFF && (instruction & (0x8 << 28))) << 11) | ((instruction >> 20) & 0x7FF); 
    } else if (this->is_s_instr == true) {
        this->immidiate = ((0x1FFFFF && (instruction & (0x8 << 28))) << 11) | (((instruction >> 25) & 0x3F) << 5) | ((instruction >> 7) & 0x1F); 
    } else if (this->is_b_instr == true) {
        this->immidiate = ((0xFFFFF && (instruction & (0x8 << 28))) << 12) | (((instruction >> 7) & 0x1) << 11) | (((instruction >> 25) & 0x3F) << 5) | (((instruction >> 7) & 0xF) << 1) | 0; 
    } else if (this->is_u_instr == true) {
        this->immidiate = (((instruction >> 20) && 0xFFFFF) << 12) | 0;
    } else if (this->is_j_instr == true) {
        this->immidiate = (0xFFF && (instruction & (0x8 << 28))) | (((instruction >> 12) & 0xFF) << 12) | (((instruction >> 20) & 0x1) << 11) | (((instruction >> 21) & 0x3FF) << 1) | 0;
    } else {
        this->immidiate = 0;
    }

    // Check field validity
    this->funct7_valid = this->is_r_instr;
    this->rs2_valid = this->is_r_instr || this->is_s_instr || this->is_b_instr;
    this->rs1_valid = this->is_r_instr || this->is_s_instr || this->is_i_instr || this->is_b_instr;
    this->funct3_valid = this->is_r_instr || this->is_s_instr || this->is_i_instr || this->is_b_instr;
    this->rd_valid = (this->is_r_instr || this->is_i_instr || this->is_u_instr || this->is_j_instr) && (this->rd != 0);
    this->imm_valid = !this->is_r_instr;

    std::cout << "Extracted fields:\n";
    std::cout << "Funct7 - " << this->funct7 << "\n"; 
    std::cout << "Rs2 - " << this->rs2 << "\n";
    std::cout << "Rs1 - " << this->rs1 << "\n";
    std::cout << "Funct3 - " << this->funct3 << "\n";
    std::cout << "Rd - " << this->rd << "\n";
    std::cout << "Opcode - " << this->opcode << "\n";
    std::cout << "Immidiate - " << this->immidiate << "\n";

    // Extract the decode bits needed
    unsigned int decode_bits = (((instruction & (0x4 << 28)) << 10) | (this->funct3 << 7) | (this->opcode)) & 0x7FF;

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
