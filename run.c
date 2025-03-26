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

        set_signbit(sreg);

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
        
        if ((char)*gen_reg[dest] < 0) {
            set_statusflag(sreg, SFLAG_NEGATIVE, 1);
        } else {
            set_statusflag(sreg, SFLAG_NEGATIVE, 0);
        }

        set_signbit(sreg);

        if (*gen_reg[dest] == 0) {
            set_statusflag(sreg, SFLAG_ZERO, 1);
        } else {
            set_statusflag(sreg, SFLAG_ZERO, 0);
        }
    }

    // ADC (add with carry) instruction logic
    else if (opcode == OPCODE_ADC) {
        char src;
        char dest;
        get_5bit_dest_src(operand, &src, &dest);

        unsigned char old = (*gen_reg)[dest];

        if (*gen_reg[dest] == 0x7F) { // Handle two's complement overflow
            set_statusflag(sreg, SFLAG_TWOCOMPOVERFLOW, 1);
        } else {
            set_statusflag(sreg, SFLAG_TWOCOMPOVERFLOW, 0);
        } 

        // ADC is basically just add but with the carry flag added to the result
        (*gen_reg)[dest] = (*gen_reg)[dest] + (*gen_reg)[src] + get_statusflag(sreg, SFLAG_CARRY);

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
        
        if ((char)*gen_reg[dest] < 0) {
            set_statusflag(sreg, SFLAG_NEGATIVE, 1);
        } else {
            set_statusflag(sreg, SFLAG_NEGATIVE, 0);
        }

        set_signbit(sreg);

        if (*gen_reg[dest] == 0) {
            set_statusflag(sreg, SFLAG_ZERO, 1);
        } else {
            set_statusflag(sreg, SFLAG_ZERO, 0);
        }
    }

    // ADIW (add immediate to word) instruction logic
    else if (opcode == OPCODE_ADIW) {
        char dest = (operand >> 4) & 0b00000011;
        char value = ((operand & 0b11000000) >> 2) | (operand & 0b1111);

        unsigned short old = (*gen_reg)[dest * 2 + 24] << 8 | (*gen_reg)[dest * 2 + 24 + 1];
        unsigned short new = old + value;

        if (old == 0b0111111111111111) { // Handle two's complement overflow
            set_statusflag(sreg, SFLAG_TWOCOMPOVERFLOW, 1);
        } else {
            set_statusflag(sreg, SFLAG_TWOCOMPOVERFLOW, 0);
        } 

        (*gen_reg)[dest * 2 + 24 + 1] = new >> 8;
        (*gen_reg)[dest * 2 + 24] = new & 0xFF;
    }

    return 0;
}