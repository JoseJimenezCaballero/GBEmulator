#include <instructions.h>
#include <cpu.h>
#include <bus.h>

extern cpu_context ctx;

InstructionFunc instruction_table[256] = {0};//declare the table 

//This function will initialize the table and add the opcodes to the correct address
void init_instruction_table(){
    instruction_table[0x00] = instr_nop;


    instruction_table[0xC3] = instr_jp_a16;
}

//actual opcode function definitions below:


//00
void instr_nop(){
    //does nothing
    ctx.regs.pc++;
    ctx.cycles += 4;
}


//C3
void instr_jp_a16(){
    //since its a 16 bit register we need to read low and high
    u8 low = bus_read(ctx.regs.pc++); //read the next 8 bytes and increment pc
    u8 high = bus_read(ctx.regs.pc++);
    u16 addr = (high << 8) | low; //combine and form full address

    //now we just update pc to the new address 
    ctx.regs.pc = addr;

    ctx.cycles += 16;

}