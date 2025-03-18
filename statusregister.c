#include "misc.h"
#include <stdlib.h>
#include <stdio.h>

void set_statusflag(unsigned char *sreg, int flag, unsigned int value) {
    if (value != 0 && value != 1) {
        fprintf(stderr, "FATAL ERROR: An invalid value was provided at \
            void set_statusflag(unsigned char *sreg, int flag, unsigned int value). This is not meant to happen. \
            If you are not a developer, please create an issue on the github page.\n");
        exit(1);
    }

    switch(flag) {
        case SFLAG_CARRY:
            *sreg = *sreg | value;
            break;
        case SFLAG_ZERO:
            value = value << 1;
            *sreg = *sreg | value;
            break;
        case SFLAG_NEGATIVE:
            value = value << 2;
            *sreg = *sreg | value;
            break;
        case SFLAG_TWOCOMPOVERFLOW:
            value = value << 3;
            *sreg = *sreg | value;
            break;
        case SFLAG_SIGNBIT:
            value = value << 4;
            *sreg = *sreg | value;
            break;
        case SFLAG_HALFCARRY:
            value = value << 5;
            *sreg = *sreg | value;
            break;
        case SFLAG_BITCOPYSTORAGE:
            value = value << 6;
            *sreg = *sreg | value;
            break;
        case SFLAG_GLOBALINTERRUPTENABLE:
            value = value << 7;
            *sreg = *sreg | value;
            break;
    }
}

unsigned int get_statusflag(unsigned char *sreg, char flag) {
    switch(flag) {
        case SFLAG_CARRY:
            return *sreg & 0b00000001;
        case SFLAG_ZERO:
            return (*sreg >> 1) & 0b00000001;
        case SFLAG_NEGATIVE:
            return (*sreg >> 2) & 0b00000001;
        case SFLAG_TWOCOMPOVERFLOW:
            return (*sreg >> 3) & 0b00000001;
        case SFLAG_SIGNBIT:
            return (*sreg >> 4) & 0b00000001;
        case SFLAG_HALFCARRY:
            return (*sreg >> 5) & 0b00000001;
        case SFLAG_BITCOPYSTORAGE:
            return (*sreg >> 6) & 0b00000001;
        case SFLAG_GLOBALINTERRUPTENABLE:
            return (*sreg >> 7) & 0b00000001;
    }

    fprintf(stderr, "FATAL ERROR: An invalid value was provided at \
        unsigned int get_statusflag(unsigned char *sreg, char flag). This is not meant to happen. \
        If you are not a developer, please create an issue on the github page.\n");
    exit(1);
}

void print_statusflags(unsigned char* sreg) {
    printf("\nStatus Flags\n--------------------------------\n");
    printf("%-40s %d\n", "Carry Flag (C)", get_statusflag(sreg, SFLAG_CARRY));
    printf("%-40s %d\n", "Zero Flag (Z)", get_statusflag(sreg, SFLAG_ZERO));
    printf("%-40s %d\n", "Negative Flag (N)", get_statusflag(sreg, SFLAG_NEGATIVE));
    printf("%-40s %d\n", "Two's Complement Overflow Flag (V)", get_statusflag(sreg, SFLAG_TWOCOMPOVERFLOW));
    printf("%-40s %d\n", "Sign Bit (S)", get_statusflag(sreg, SFLAG_SIGNBIT));
    printf("%-40s %d\n", "Half Carry flag (H)", get_statusflag(sreg, SFLAG_HALFCARRY));
    printf("%-40s %d\n", "Bit Copy Storage (T)", get_statusflag(sreg, SFLAG_BITCOPYSTORAGE));
    printf("%-40s %d\n", "Global Interrupt Enable (I)", get_statusflag(sreg, SFLAG_GLOBALINTERRUPTENABLE));
    printf("\n");
}