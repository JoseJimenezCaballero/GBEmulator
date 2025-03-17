
#include <common.h>
#include <cpu.h>

//will contain the map definition for the opcodes and the fucntions too for each opcode

//defining a function pointer type for cpu instructions
typedef void (*InstructionFunc)();

//instruction lookup table(one per opcode)
extern InstructionFunc instruction_table[256];

//to init instruction table
void init_instruction_table();


//fucntion prototypes for CPU instructions go here:
void instr_nop();//00


void instr_jp_a16();//C3