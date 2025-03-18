#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "misc.h"
#include "instructionset.h"

// The AtMega2560 microcontroller has 32 general purpose registers
#define GEN_REG_NUM 32

int main(int argc, char *argv[]) {
    printf("AtMega2560 microcontroller emulator\nType help to view a list of commands.\n\n");

    // 8 bit general purpose registers
    unsigned char gen_reg[GEN_REG_NUM];
    
    // Initialize general purpose registers
    for (int i = 0; i < GEN_REG_NUM; i++) {
        gen_reg[i] = 0;
    }

    // Initialize status register
    unsigned char sreg = 0b00000000;

    // Initialize breakpoint storage
    int* breakpoints = malloc(0);

    // Initialize variables for decoding
    FILE *stream;
    char *line = NULL;
    int inslen = 0;
    instruction_t *instructions = NULL;
    size_t len = 0; 
    ssize_t nread; // string length

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    stream = fopen(argv[1], "r");
    if (stream == NULL) {
        perror("fopen");
        return 1;
    }

    // The .hex file is decoded line by line and decoded instructions are saved in the instruction memory
    // As of right now, this program does not verify whether the provided file is a valid text
    // file formatted in the intel hex format. This verification will be implemented later.
    while ((nread = getline(&line, &len, stream)) != -1) {
        char* new_line = NULL;
        size_t len = strcut(&new_line, 9, nread - 4, line);
        instructions = decode(&instructions, new_line, len, &inslen);
        free(new_line);
    }

    printf("Program %s flashed successfully!\nType sf to view the loaded program.\n\n", argv[1]);

    // Start the program's main loop
    while (1) {
        char params[255];
        int input = run_shell(params);
        if (input == COM_UNKNOWN) {
            printf("Invalid command entered. Type h to view a list of valid commands.\n");
        } else if (input == COM_EXIT) {
            printf("Exiting...\n");

            // Cleanup
            free(breakpoints);
            free(instructions);
            free(line);
            fclose(stream);
            return 0;
        } else if (input == COM_SHOWFLASH) {
            // TODO: Wrap this functionality in a function to clean this segment up
            printf("\nInstruction memory:\n--------------------------------\n");
            for (int i = 0; i < inslen / (int)sizeof(instruction_t); i++) {
                if (instructions[i].opcode == OPCODE_INC) {
                    printf("%d: inc r%02d\n", i, instructions[i].operand);
                } else if (instructions[i].opcode == OPCODE_RJMP) {
                    printf("%d: rjmp .%d\n", i, instructions[i].operand);
                } 
            }
            printf("\n");
        } else if (input == COM_SHOWGENREGS) {
            print_genregs_table(gen_reg, GEN_REG_NUM);
        } else if (input == COM_RUN) {
            //TODO: Implement breakpoints
            for (int insptr = 0; insptr < inslen / (int)sizeof(instruction_t); insptr++) {
                run_instruction(instructions[insptr].opcode, instructions[insptr].operand, &gen_reg, &insptr, &sreg);
            }
            printf("Program %s executed successfully.\n", argv[1]);
        } else if (input == COM_RESET) {
            // Reset registers
            for (int i = 0; i < GEN_REG_NUM; i++) {
                gen_reg[i] = 0;
            }
            printf("Processor has been reset to its default state.\n");
        } else if (input == COM_HELP) {
            show_help();
        } else if (input == COM_SHOWSFLAGS) {
            print_statusflags(&sreg);
        }
    }
}