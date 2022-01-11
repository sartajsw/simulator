// Interface to a simple memory implemented as a large contigious block without
// distinction between pages, page sections, virtual memory, etc.
class Memory {
private:
    // Pointer to the allocated memory
    char *memory = nullptr;

    // Size of the allocated memory
    int mem_size = 0;

public:
    // Constructor
    Memory(int size);
    // Destructor
    ~Memory();
};