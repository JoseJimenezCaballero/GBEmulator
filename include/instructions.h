
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
void ld_bc_d16();//01
void instr_ld_b_d8();//06
void instr_ld_c_d8();//0E

void jr_r8();//18

void instr_ld_hl_d16();//21
void instr_jr_z_r8();//28

void ld_a_d8();//3E

void instr_ld_la();//6F

void instr_xor_a();//AF

void instr_jp_a16();//C3
void insr_call_a16();//CD

void instr_ldh_a8_a();//E0
void instr_ld_a16_a();//EA

void instr_di();//F3
void instr_ld_a_a16();//FA
void instr_cp_d8();//FE