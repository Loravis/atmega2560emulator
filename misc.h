typedef struct {
    unsigned int opcode;
    unsigned int operand;
} instruction_t;

typedef enum {
    COM_UNKNOWN = -1,
    COM_EXIT = 0,
    COM_SHOWFLASH = 1,
    COM_SHOWGENREGS = 2,
    COM_RUN = 3,
    COM_RESET = 4,
    COM_HELP = 5
} user_commands_t;

instruction_t* decode(instruction_t** instructions, char* str, unsigned long len, int* inslen);
int strcut(char** dest, unsigned int start, unsigned int end, char * str);

int print_genregs(unsigned char* registers, int num);
int print_genregs_table(unsigned char* registers, int num);

int run_shell();
void show_help();

int run_instruction(unsigned int opcode, unsigned int operand, unsigned char(*gen_reg)[32], int* insptr);