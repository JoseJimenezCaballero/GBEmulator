#pragma once

#include "common.h"


/*
registers:
AF	A	-	Accumulator & Flags
BC	B	C	BC
DE	D	E	DE
HL	H	L	HL
SP	-	-	Stack Pointer
PC	-	-	Program Counter/Pointer
*/

typedef struct { //contain all registers in a seperate struct for organization
    u8 a;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 f;
    u8 h;
    u8 l;
    u16 pc;//program counter
    u16 sp;//stack pointer

} cpu_registers;

cpu_registers *cpu_get_regs();

typedef struct {
    cpu_registers regs;

    u8 opcode; //current opcode being executed
    u32 cycles; //cycles 

    bool ime; //interrupt master flag
    bool halted; //halt state
    bool stepping;

} cpu_context;


void cpu_init();
bool cpu_step();