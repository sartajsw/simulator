#include "memory.h"

Memory::Memory(int size) {
    std::cout << "Memory Constructor \n";

    // Set the size of the memory
    this->mem_size = size;

    // Allocate memory
    this->memory = new unsigned char[this->mem_size];
}

Memory::~Memory() {}

unsigned int Memory::read(unsigned int address) {
    // Calculate offset of address from starting point
    unsigned int offset = address - this->address_start;

    unsigned int data = 0;

    // If offset within range
    if (offset < this->mem_size) {
        data = 
            (this->memory[offset+3] << 24) |
            (this->memory[offset+2] << 16) |
            (this->memory[offset+1] <<  8) |
            (this->memory[offset+0] <<  0);
    }

    return data;
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

void Memory::load_hex() {
    std::cout << "Loading hex program into memory \n";

    // Open file
    std::ifstream file ("tests/sum_of_num.hex", std::ios::in);
    
    // Check file
    if (!file.is_open()) {
        std::cout << "File could not be opened! \n";
        return;
    }

    // To read from file
    unsigned int word;
    unsigned int i = 0;
    std::string line;
    while (std::getline(file, line)) {
        // Convert the string hex value
        word = std::stol(line, nullptr, 16);
        // Write into memory
        this->write(i + this->address_start, word);
        // Increment address
        i+=4;
    }

    // Clean up
    file.close();
}

unsigned int Memory::get_start() {
    return address_start;
}