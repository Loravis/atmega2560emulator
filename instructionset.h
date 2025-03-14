// atmega2560 instruction set

typedef enum {
    OPCODE_INC = 0b1001010000000011,
    OPCODE_RJMP = 0b1100000000000000
} avr_opcode_t;

typedef enum {
    OPCODEMASK_INC = 0b1111111000001111,
    OPCODEMASK_RJMP = 0b1111000000000000
} avr_opcodemask_t;