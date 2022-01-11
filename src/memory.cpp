#include "memory.h"

Memory::Memory(int size) {
    // Set the size of the memory
    this->mem_size = size;

    // Allocate memory
    this->memory = new char[this->mem_size];
}

Memory::~Memory() {}