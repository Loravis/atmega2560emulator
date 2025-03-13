#include <stdio.h>
#include <string.h>
#include "misc.h"

// Take user input, verify that it's valid and return a code based on what was entered
int run_shell() {
    char buf[255]; // User input buffer
    printf("(emul): ");
    scanf("%254s", buf);
    
    if (strcmp(buf, "exit") == 0) { // Exit
        return COM_EXIT;
    } else if (strcmp(buf, "sf") == 0) { // show program stored in flash
        return COM_SHOWFLASH;
    } else if (strcmp(buf, "sgr") == 0) { // show content of general purpose registers
        return COM_SHOWGENREGS;
    } else if (strcmp(buf, "run") == 0) { // run program
        return COM_RUN;
    } else if (strcmp(buf, "reset") == 0) { // reset registers, storage, etc...
        return COM_RESET;
    }

    return COM_UNKNOWN;
}