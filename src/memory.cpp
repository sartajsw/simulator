#include "memory.h"

Memory::Memory(int size) {
    // Set the size of the memory
    this->mem_size = size;

    // Allocate memory
    this->memory = new char[this->mem_size];
}

Memory::~Memory() {}

unsigned int Memory::read(unsigned int address) {
    // Calculate offset of address from starting point
    unsigned int offset = address - this->address_start;

    // If offset within range
    if (offset < this->mem_size) {
        return
            (this->memory[offset+3] << 24) |
            (this->memory[offset+2] << 16) |
            (this->memory[offset+1] <<  8) |
            (this->memory[offset+0] <<  0);
    }

    return 0;
}


void Memory::write(unsigned int address, unsigned int data) {
    // Calculate offset of address from starting point
    unsigned int offset = address - this->address_start;

    // If offset within range
    if (offset < this->mem_size) {
        this->memory[offset+3] = (data >> 24) & 0xFF;
        this->memory[offset+2] = (data >> 16) & 0xFF;
        this->memory[offset+1] = (data >>  8) & 0xFF;
        this->memory[offset+0] = (data >>  0) & 0xFF;
    }
}

void Memory::load_program() {
    // To read from file
    unsigned int word;
    char *memblock = new char [4];

    // Open file
    std::ifstream file ("../tests/program", std::ios::in | std::ios::binary);
    
    // Check file
    if (!file.is_open()) {
        std::cout << "File could not be opened! \n";
        return;
    }

    // Read till EOF
    unsigned int i = 0;
    while (!file.eof()) {
        // Read an instruction (4 bytes)
        file.read (memblock, 4);
        // Copy value into word
        sscanf(memblock, "%d", &word);
        // Write into memory
        this->write(i + this->address_start, word);
        // Increment address
        i+=4;
    }

    // Clean up
    file.close();
    delete[] memblock;
}

unsigned int Memory::get_start() {
    return address_start;
}