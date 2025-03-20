#include "misc.h"
#include "instructionset.h"

// Instructions are run according to how they're meant to run on the atmega2560. 
// This function's functionality is derived from the AVR Instruction Set Manual, linked below.
// https://ww1.microchip.com/downloads/en/DeviceDoc/AVR-Instruction-Set-Manual-DS40002198A.pdf
int run_instruction(unsigned int opcode, unsigned int operand, unsigned char(*gen_reg)[32], int* insptr, unsigned char* sreg) {
    // INC instruction logic
    if (opcode == OPCODE_INC) {
        if (*gen_reg[operand] == 0x7F) { // Handle two's complement overflow
            set_statusflag(sreg, SFLAG_TWOCOMPOVERFLOW, 1);
        } else {
            set_statusflag(sreg, SFLAG_TWOCOMPOVERFLOW, 0);
        } 

        (*gen_reg)[operand]++; // Perform the increment operation

        if ((char)*gen_reg[operand] < 0) {
            set_statusflag(sreg, SFLAG_NEGATIVE, 1);
        } else {
            set_statusflag(sreg, SFLAG_NEGATIVE, 0);
        }

        // Set sign bit
        set_statusflag(sreg, SFLAG_SIGNBIT, get_statusflag(sreg, SFLAG_TWOCOMPOVERFLOW) ^ get_statusflag(sreg, SFLAG_NEGATIVE));

        if (*gen_reg[operand] == 0) {
            set_statusflag(sreg, SFLAG_ZERO, 1);
        } else {
            set_statusflag(sreg, SFLAG_ZERO, 0);
        }
    } 
    // RJMP instruction logic
    else if (opcode == OPCODE_RJMP) {
        (*insptr) = *insptr + operand;
    }
    // ADD instruction logic
    else if (opcode == OPCODE_ADD) {
        char src;
        char dest;
        get_5bit_dest_src(operand, &src, &dest);

        unsigned char old = (*gen_reg)[dest];

        if (*gen_reg[dest] == 0x7F) { // Handle two's complement overflow
            set_statusflag(sreg, SFLAG_TWOCOMPOVERFLOW, 1);
        } else {
            set_statusflag(sreg, SFLAG_TWOCOMPOVERFLOW, 0);
        } 

        (*gen_reg)[dest] = (*gen_reg)[dest] + (*gen_reg)[src];

        // Check for half carry
        if (((old & (*gen_reg)[dest]) & 0b1000) >> 3 == 1) {
            set_statusflag(sreg, SFLAG_HALFCARRY, 1);
        } else {
            set_statusflag(sreg, SFLAG_HALFCARRY, 0);
        }

        // Check for carry
        if (((old & (*gen_reg)[dest]) & 0b10000000) >> 7 == 1) {
            set_statusflag(sreg, SFLAG_HALFCARRY, 1);
        } else {
            set_statusflag(sreg, SFLAG_HALFCARRY, 0);
        }
        
        // Set sign bit
        set_statusflag(sreg, SFLAG_SIGNBIT, get_statusflag(sreg, SFLAG_TWOCOMPOVERFLOW) ^ get_statusflag(sreg, SFLAG_NEGATIVE));

        if ((char)*gen_reg[dest] < 0) {
            set_statusflag(sreg, SFLAG_NEGATIVE, 1);
        } else {
            set_statusflag(sreg, SFLAG_NEGATIVE, 0);
        }

        if (*gen_reg[dest] == 0) {
            set_statusflag(sreg, SFLAG_ZERO, 1);
        } else {
            set_statusflag(sreg, SFLAG_ZERO, 0);
        }
    }
    return 0;
}