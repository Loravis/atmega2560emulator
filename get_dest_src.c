#include "misc.h"

void get_5bit_dest_src(unsigned int operand, char *src, char *dest) {
    (*dest) = operand >> 4;
    (*dest) = *dest & 0b0000000000011111;

    (*src) = ((operand & 0b0000001000000000) >> 5 | operand);
}