ADDI x14, x0, 0
ADDI x12, x0, 0xA
ADDI x13, x0, 1
ADD x14, x13, x14
ADDI x13, x13, 1
BLT x13, x12, -0x8
ADDI x30, x14, 42
BGE x0, x0, 0