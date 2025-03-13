#include "misc.h"
#include "instructionset.h"

int run_instruction(unsigned int opcode, unsigned int operand, unsigned char(*gen_reg)[32]) {
    if (opcode == OPCODE_INC) {
        (*gen_reg)[operand]++;
    }
    return 0;
}