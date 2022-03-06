// #include "core.h"
#include "pipe.h"

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

    // Initialize Pipeline Stages
    this->Fetch(this);
    this->Decode(this);
    this->Execute(this);
    this->MemoryAccess(this);
    this->Writeback(this);

    // Set run flag
    run = true;
}

Core::~Core() {}

void Core::tick() {
    // If run flag is set
    if (run == true) {
        this->Fetch->tick();
        this->Decode->tick();
        this->Execute->tick();
        this->MemoryAccess->tick();
        this->Writeback->tick();
    }

    // Increment intruction count
    this->instr_count++;
}

void Core::flush(int stage) {
    switch(stage) {
        // From Decode Stage
        case 2:
            this->Fetch->reset();
            this->Decode->reset();
            break;
        // From Execute Stage
        case 3:
            this->Fetch->reset();
            this->Decode->reset();
            this->Execute->reset();
            break;
        default:
            std::cout << "Flush called from unsupported stage! \n";
    }
}

bool Core::get_run() {
    return this->run;
}

void Core::print_RF() {
    std::cout << std::dec << "RF: ";
    for(int i = 0; i < this->num_regs; i++)
        std::cout << i << ":" << this->RF[i] << " ";
    std::cout << std::hex << std::endl;
}

void Core::increment_PC() {
    this->PC += 4;
}

unsigned int Core::read_PC() {
    return this->ptrMemory->read(this->PC);
}
