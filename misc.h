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
    COM_HELP = 5,
    COM_SHOWSFLAGS = 6,
    COM_ADDBREAKPOINT = 7
} user_commands_t;

typedef enum {
    SFLAG_CARRY = 0,
    SFLAG_ZERO = 1,
    SFLAG_NEGATIVE = 2,
    SFLAG_TWOCOMPOVERFLOW = 3,
    SFLAG_SIGNBIT = 4,
    SFLAG_HALFCARRY = 5,
    SFLAG_BITCOPYSTORAGE = 6,
    SFLAG_GLOBALINTERRUPTENABLE = 7
} statusflags_t;

instruction_t* decode(instruction_t** instructions, char* str, unsigned long len, int* inslen);
int strcut(char** dest, unsigned int start, unsigned int end, char * str);

int print_genregs_table(unsigned char* registers, int num);

int run_shell(char* strparams, int* numparam);
void show_help();

int run_instruction(unsigned int opcode, unsigned int operand, unsigned char(*gen_reg)[32], int* insptr, unsigned char* sreg);
void disasm_insmem(instruction_t* instructions, int* inslen);
void get_5bit_dest_src(unsigned int operand, char* src, char* dest);

void set_statusflag(unsigned char* sreg, int flag, unsigned int value);
unsigned int get_statusflag(unsigned char* sreg, char flag);
void print_statusflags(unsigned char* sreg);

/*
Functions for setting status flags during instruction execution
*/
void set_signbit(unsigned char* sreg);
void check_negative_zero(unsigned char* sreg, unsigned char* gen_reg, char dest);