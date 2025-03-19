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

    // Initialize instruction pointer
    int insptr = 0;

    // Initialize breakpoint storage
    int breakpoint_n = 0;
    int* breakpoints = malloc(breakpoint_n);
    int breakpoint_active = 0; // Used to determine whether the run command should reset the instruction pointer or not.

    // Initialize variables for decoding
    FILE *stream;
    char *line = NULL; // String containing a line of the read file
    int inslen = 0;
    instruction_t *instructions = NULL;
    size_t len = 0; 
    ssize_t nread; // string character count

    // Check if a valid number of arguments was provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    // Open file stream for reading
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

    // The program's main loop repeatedly opens a shell prompt and processes commands provided by the user.
    while (1) {
        // These two variables are defined to hold any parameters that are provided with certain shell commands
        char params[255];
        int numparam = -1;

        int input = run_shell(params, &numparam);
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

            // If there's no active breakpoint, the program starts from the beginning.
            // If there is an active breakpoint, the breakpoint is deactivated and the program continues where it left off
            if (breakpoint_active == 0) {
                insptr = 0;
            } else {
                breakpoint_active = 0;
            }
            
            // Program execution loop, calls the run_instruction method repeatedly until the end of the program is reached
            for (; insptr < inslen / (int)sizeof(instruction_t); insptr++) {
                run_instruction(instructions[insptr].opcode, instructions[insptr].operand, &gen_reg, &insptr, &sreg);
                
                // Loop through the breakpoint storage and check if a breakpoint for the next instruction could be found.
                // If that's the case, the instruction pointer is incremented and the program execution loop is immediately exited.
                for (int i = 0; i < breakpoint_n; i++) {
                    if (insptr + 1 == breakpoints[i]) {
                        breakpoint_active = 1;
                        break;
                    }
                } if (breakpoint_active == 1) {
                    insptr++;
                    printf("\nReached breakpoint at instruction #%d.\n\n", insptr);
                    break;
                }
            }
            
            // Display a success message once the program fully finishes executing
            if (breakpoint_active == 0) {
                printf("Program %s executed successfully.\n", argv[1]);
            }
        } else if (input == COM_RESET) {
            // Reset registers
            for (int i = 0; i < GEN_REG_NUM; i++) {
                gen_reg[i] = 0;
            }

            // Reset instruction pointer
            insptr = 0;

            printf("Processor has been reset to its default state.\n");
        } else if (input == COM_HELP) {
            show_help();
        } else if (input == COM_SHOWSFLAGS) {
            print_statusflags(&sreg);
        } else if (input == COM_ADDBREAKPOINT && numparam >= 0) {
            breakpoint_n++;
            breakpoints = realloc(breakpoints, sizeof(int) * breakpoint_n);
            breakpoints[breakpoint_n - 1] = numparam;
            printf("Breakpoint added at instruction #%d\n", numparam);
        }
    }
}