#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "misc.h"
#include "instructionset.h"

#define OFFSET 4

instruction_t* decode(instruction_t** instructions, char *str, unsigned long len, int* inslen){
    char* part = NULL;
    for (int i = 0; i < len; i += OFFSET) {
        strcut(&part, i, i + OFFSET, str);
        int instruction = (int)strtol(part, NULL, 16);

        // Swap byte order
        int a = (instruction & 0x00FF) << 8;
        int b = (instruction & 0xFF00) >> 8;
        instruction = a | b;

        // Check for 2 byte instructions
        if ((instruction & OPCODEMASK_INC) == OPCODE_INC) {
            *inslen += sizeof(instruction_t);
            *instructions = realloc(*instructions, *inslen);
            (*instructions)[(*inslen / sizeof(instruction_t)) - 1].opcode = OPCODE_INC;
            (*instructions)[(*inslen / sizeof(instruction_t)) - 1].operand = (instruction & (~OPCODEMASK_INC)) >> 4;
        } 
        
        else if ((instruction & OPCODEMASK_RJMP) == OPCODE_RJMP) {
            *inslen += sizeof(instruction_t);
            *instructions = realloc(*instructions, *inslen);
            (*instructions)[(*inslen / sizeof(instruction_t)) - 1].opcode = OPCODE_RJMP;

            int16_t operandmask = (int16_t)~OPCODEMASK_RJMP;
            int16_t operand = instruction & operandmask;
            
            // Ensure values stored as two's complement (negative) are decoded correctly
            if ((operand & 0b100000000000) != 0) {
                operand = ((~operand & operandmask) + 1) * -1;
            } else {
                operand++;
            }

            (*instructions)[(*inslen / sizeof(instruction_t)) - 1].operand = operand;
        }
    }
    free(part);
    return *instructions;
}