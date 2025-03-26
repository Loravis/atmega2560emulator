#include <stdio.h>

void print_genregs_table(unsigned char* registers, int num) {
    printf("\nGeneral purpose registers:\n--------------------------------\n");

    for (int i = 0; i < num; i++) {
        printf("r%02d: 0x%02x", i, (int)registers[i]); // Print register and value

        if ((i + 1) % 3 == 0) { // Check if we've printed 3 columns
            printf("\n"); // Start a new row
        } else {
            printf("  "); // Add some spacing between columns
        }
    }

    if (num % 3 != 0) { // If the last row wasn't full, add a newline
        printf("\n");
    }

    printf("\n");
}