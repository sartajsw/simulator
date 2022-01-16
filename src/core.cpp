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
    this->instr_count++;
}

void Core::process_instruction() {
    /* Fetch */
    this->instruction = this->ptrMemory->read(this->PC);
    std::cout << "Instruction: " << this->instruction << std::endl;

    /* Decode */
    // Get instruction format
    unsigned int masked_instr = (this->instruction & (0x1F << 2)) >> 2;
    
    std::cout << "Instruction Format: ";
    this->instr_format = InstructionFormat::Invalid;
    switch (masked_instr) {
        case 0b01011:
        case 0b10100:
        case 0b01100:
        case 0b01110:
            this->instr_format = InstructionFormat::R;
            std::cout << "R\n";
            break;
        case 0b00000:
        case 0b00001:
        case 0b00100:
        case 0b00110:
        case 0b11001:
            this->instr_format = InstructionFormat::I;
            std::cout << "I\n";
            break;
        case 0b01000:
        case 0b01001:
            this->instr_format = InstructionFormat::S;
            std::cout << "S\n";
            break;
        case 0b11000:
            this->instr_format = InstructionFormat::B;
            std::cout << "B\n";
            break;
        case 0b00101:
        case 0b01101:
            this->instr_format = InstructionFormat::U;
            std::cout << "U\n";
            break;
        case 0b11011:
            this->instr_format = InstructionFormat::J;
            std::cout << "J\n";
            break;
        default:
            std::cerr << "Instruction format invalid! \n";
            exit(0);
    }

    // Extract fields
    this->funct7  = (this->instruction >> 25) & 0x7F;
    this->rs2 = (this->instruction >> 20) & 0x1F;
    this->rs1 = (this->instruction >> 15) & 0x1F;
    this->funct3 = (this->instruction >> 12) & 0x7;
    this->rd = (this->instruction >> 7) & 0x1F;
    this->opcode = this->instruction & 0x7F;

    // Extract immidiate value
    switch (this->instr_format) {
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
    this->funct7_valid = (this->instr_format == InstructionFormat::R);
    this->rs2_valid = (this->instr_format == InstructionFormat::R) || 
                      (this->instr_format == InstructionFormat::S) || 
                      (this->instr_format == InstructionFormat::B);
    this->rs1_valid = (this->instr_format == InstructionFormat::R) || 
                      (this->instr_format == InstructionFormat::S) || 
                      (this->instr_format == InstructionFormat::I) || 
                      (this->instr_format == InstructionFormat::B);
    this->funct3_valid = (this->instr_format == InstructionFormat::R) || 
                         (this->instr_format == InstructionFormat::S) || 
                         (this->instr_format == InstructionFormat::I) || 
                         (this->instr_format == InstructionFormat::B);
    this->rd_valid = ((this->instr_format == InstructionFormat::R) || 
                      (this->instr_format == InstructionFormat::I) || 
                      (this->instr_format == InstructionFormat::U) || 
                      (this->instr_format == InstructionFormat::J)) && 
                     (this->rd != 0);
    this->imm_valid = !(this->instr_format == InstructionFormat::R);

    std::cout << "Extracted fields:\n";
    std::cout << "Funct7 - " << this->funct7 << "\n"; 
    std::cout << "Rs2 - " << this->rs2 << "\n";
    std::cout << "Rs1 - " << this->rs1 << "\n";
    std::cout << "Funct3 - " << this->funct3 << "\n";
    std::cout << "Rd - " << this->rd << "\n";
    std::cout << "Opcode - " << this->opcode << "\n";
    std::cout << "Immidiate - " << this->immidiate << "\n";

    // Extract the decode bits needed
    this->decode_bits = ((this->instruction >> 0x14) & 0x400) |
                               ((this->funct3 << 0x7) & 0x380) |
                               (this->opcode & 0x7F);

    std::cout << "Intruction Type: ";
    this->instr_type = Instruction::Invalid;
    switch(this->decode_bits) {
        case 0b0'000'1100011:
        case 0b1'000'1100011:
            this->instr_type = Instruction::BEQ;
            std::cout << "BEQ\n";
            break;
        case 0b0'001'1100011:
        case 0b1'001'1100011:
            this->instr_type = Instruction::BNE;
            std::cout << "BNE\n";
            break;
        case 0b0'100'1100011:
        case 0b1'100'1100011:
            this->instr_type = Instruction::BLT;
            std::cout << "BLT\n";
            break;
        case 0b0'101'1100011:
        case 0b1'101'1100011:
            this->instr_type = Instruction::BGE;
            std::cout << "BGE\n";
            break;
        case 0b0'110'1100011:
        case 0b1'110'1100011:
            this->instr_type = Instruction::BLTU;
            std::cout << "BLTU\n";
            break;
        case 0b0'111'1100011:
        case 0b1'111'1100011:
            this->instr_type = Instruction::BGEU;
            std::cout << "BGEU\n";
            break;
        case 0b0'000'0010011:
        case 0b1'000'0010011:
            this->instr_type = Instruction::ADDI;
            std::cout << "ADDI\n";
            break;
        case 0b0'000'0110011:
            this->instr_type = Instruction::ADD;
            std::cout << "ADD\n";
            break;
        case 0b0'000'0110111:
        case 0b0'001'0110111:
        case 0b0'010'0110111:
        case 0b0'011'0110111:
        case 0b0'100'0110111:
        case 0b0'101'0110111:
        case 0b0'110'0110111:
        case 0b0'111'0110111:
        case 0b1'000'0110111:
        case 0b1'001'0110111:
        case 0b1'010'0110111:
        case 0b1'011'0110111:
        case 0b1'100'0110111:
        case 0b1'101'0110111:
        case 0b1'110'0110111:
        case 0b1'111'0110111:
            this->instr_type = Instruction::LUI;
            std::cout << "LUI\n";
            break;
        case 0b0'000'0010111:
        case 0b0'001'0010111:
        case 0b0'010'0010111:
        case 0b0'011'0010111:
        case 0b0'100'0010111:
        case 0b0'101'0010111:
        case 0b0'110'0010111:
        case 0b0'111'0010111:
        case 0b1'000'0010111:
        case 0b1'001'0010111:
        case 0b1'010'0010111:
        case 0b1'011'0010111:
        case 0b1'100'0010111:
        case 0b1'101'0010111:
        case 0b1'110'0010111:
        case 0b1'111'0010111:
            this->instr_type = Instruction::AUIPC;
            std::cout << "AUIPC\n";
            break;
        case 0b0'000'1101111:
        case 0b0'001'1101111:
        case 0b0'010'1101111:
        case 0b0'011'1101111:
        case 0b0'100'1101111:
        case 0b0'101'1101111:
        case 0b0'110'1101111:
        case 0b0'111'1101111:
        case 0b1'000'1101111:
        case 0b1'001'1101111:
        case 0b1'010'1101111:
        case 0b1'011'1101111:
        case 0b1'100'1101111:
        case 0b1'101'1101111:
        case 0b1'110'1101111:
        case 0b1'111'1101111:
            this->instr_type = Instruction::JAL;
            std::cout << "JAL\n";
            break;
        case 0b0'000'1100111:
        case 0b1'000'1100111:
            this->instr_type = Instruction::JALR;
            std::cout << "JALR\n";
            break;
        case 0b0'010'0010011:
        case 0b1'010'0010011:
            this->instr_type = Instruction::SLTI;
            std::cout << "SLTI\n";
            break;
        case 0b0'011'0010011:
        case 0b1'011'0010011:
            this->instr_type = Instruction::SLTIU;
            std::cout << "SLTIU\n";
            break;
        case 0b0'100'0010011:
        case 0b1'100'0010011:
            this->instr_type = Instruction::XORI;
            std::cout << "XORI\n";
            break;
        case 0b0'110'0010011:
        case 0b1'110'0010011:
            this->instr_type = Instruction::ORI;
            std::cout << "ORI\n";
            break;
        case 0b0'111'0010011:
        case 0b1'111'0010011:
            this->instr_type = Instruction::ANDI;
            std::cout << "ADDI\n";
            break;
        case 0b0'001'0010011:
            this->instr_type = Instruction::SLLI;
            std::cout << "SLLI\n";
            break;
        case 0b0'101'0010011:
            this->instr_type = Instruction::SRLI;
            std::cout << "SRLI\n";
            break;
        case 0b1'101'0010011:
            this->instr_type = Instruction::SRAI;
            std::cout << "SRAI\n";
            break;
        case 0b1'000'0110011:
            this->instr_type = Instruction::SUB;
            std::cout << "SUB\n";
            break;
        case 0b0'001'0110011:
            this->instr_type = Instruction::SLL;
            std::cout << "SLL\n";
            break;
        case 0b0'010'0110011:
            this->instr_type = Instruction::SLT;
            std::cout << "SLT\n";
            break;
        case 0b0'011'0110011:
            this->instr_type = Instruction::SLTU;
            std::cout << "SLTU\n";
            break;
        case 0b0'100'0110011:
            this->instr_type = Instruction::XOR;
            std::cout << "XOR\n";
            break;
        case 0b0'101'0110011:
            this->instr_type = Instruction::SRL;
            std::cout << "SRL\n";
            break;
        case 0b1'101'0110011:
            this->instr_type = Instruction::SRA;
            std::cout << "SRA\n";
            break;
        case 0b0'110'0110011:
            this->instr_type = Instruction::OR;
            std::cout << "OR\n";
            break;
        case 0b0'111'0110011:
            this->instr_type = Instruction::AND;
            std::cout << "AND\n";
            break;
        case 0b0'000'0000011:
        case 0b0'001'0000011:
        case 0b0'010'0000011:
        case 0b0'011'0000011:
        case 0b0'100'0000011:
        case 0b0'101'0000011:
        case 0b0'110'0000011:
        case 0b0'111'0000011:
        case 0b1'000'0000011:
        case 0b1'001'0000011:
        case 0b1'010'0000011:
        case 0b1'011'0000011:
        case 0b1'100'0000011:
        case 0b1'101'0000011:
        case 0b1'110'0000011:
        case 0b1'111'0000011:
            this->instr_type = Instruction::LOAD;
            std::cout << "LOAD\n";
            break;
        default:
            std::cerr << "Instruction not supported!\n";
            exit(0);
    }

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
