#include<iostream>
#include<fstream>

// Interface to a simple memory implemented as a large contigious block without
// distinction between pages, page sections, virtual memory, etc.
class Memory {
private:
    // Start of memory addressses
    static const int address_start = 0x00400000;

    // Pointer to the allocated memory
    char *memory = nullptr;

    // Size of the allocated memory
    int mem_size = 0;

public:
    // Constructor
    Memory(int size);
    // Destructor
    ~Memory();

    // Read from address
    unsigned int read(unsigned int address);
    // Write from address
    void write(unsigned int address, unsigned int data);

    // Load the program
    void load_program();

    // Get the address for the PC
    unsigned int get_start();
};