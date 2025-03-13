// atmega2560 instruction set

typedef enum {
    OPCODE_JMP = 0x0C94
} avr_opcode_2byte_t; // Opcodes for 4 byte instructions

typedef enum {
    OPCODE_INC = 0x0390
} avr_opcode_1byte_t; // Opcodes for 2 byte instructions