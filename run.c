#include "misc.h"
#include "instructionset.h"

int run_instruction(unsigned int opcode, unsigned int operand, unsigned char(*gen_reg)[32], int* insptr) {
    if (opcode == OPCODE_INC) {
        (*gen_reg)[operand]++;
    } else if (opcode == OPCODE_RJMP) {
        (*insptr) = *insptr + operand;
    }
    return 0;
}