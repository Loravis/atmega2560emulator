#include "misc.h"
#include "instructionset.h"
#include <stdio.h>

// Disassemble the contents of the instruction memory and orderly print out the disassembled instructions 
void disasm_insmem(instruction_t *instructions, int *inslen) {
    printf("\nInstruction memory:\n--------------------------------\n");
    for (int i = 0; i < *inslen / (int)sizeof(instruction_t); i++) {
        if (instructions[i].opcode == OPCODE_INC) {
            printf("%d: inc r%02d\n", i, instructions[i].operand);
        } else if (instructions[i].opcode == OPCODE_RJMP) {
            printf("%d: rjmp .%d\n", i, instructions[i].operand);
        } else if (instructions[i].opcode == OPCODE_ADD) {
            char src;
            char dest;
            get_5bit_dest_src(instructions[i].operand, &src, &dest);

            printf("%d: add r%02d, r%02d\n", i, dest, src);
        } else if (instructions[i].opcode == OPCODE_ADC) {
            char src;
            char dest;
            get_5bit_dest_src(instructions[i].operand, &src, &dest);

            printf("%d: adc r%02d, r%02d\n", i, dest, src);
        } else if (instructions[i].opcode == OPCODE_ADIW) {
            char dest = (instructions[i].operand >> 4) & 0b00000011;
            char value = ((instructions[i].operand & 0b11000000) >> 2) | (instructions[i].operand & 0b1111);

            printf("%d: adiw r%02d, %d\n", i, dest * 2 + 24, value);
        }
    }
    printf("\n");
}