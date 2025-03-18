#include <instructions.h>
#include <cpu.h>
#include <bus.h>

extern cpu_context ctx;

InstructionFunc instruction_table[256] = {0};//declare the table 

//This function will initialize the table and add the opcodes to the correct address
void init_instruction_table(){
    instruction_table[0x00] = instr_nop;
    instruction_table[0x6F] = instr_ld_la;
    instruction_table[0xC3] = instr_jp_a16;
    instruction_table[0xFA] = instr_ld_a_a16;
    instruction_table[0xFE] = instr_cp_d8;
}

//actual opcode function definitions below:


//00
void instr_nop(){
    //does nothing
    ctx.cycles += 4;
}

//6F
void instr_ld_la(){
    ctx.regs.l = ctx.regs.a;
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

//FA
void instr_ld_a_a16(){
    //load onto a the 16 bit value
    u8 low = bus_read(ctx.regs.pc++);
    u8 high = bus_read(ctx.regs.pc++);
    u16 address = (high << 8) | low;
    ctx.regs.a = bus_read(address);
    ctx.cycles += 16;

}

//FE
void instr_cp_d8(){//compare if a is ==, <, or > than the immediate value and updating flags
    u8 value = bus_read(ctx.regs.pc++);
    ctx.regs.f = 0;//clear the flag

    if(ctx.regs.a == value){    //z flag
        ctx.regs.f |= FLAG_Z;
    }
    ctx.regs.f |= FLAG_N;//N flag is 1
    if((ctx.regs.a & 0x0F) < (value & 0x0F)){   //H flag is set if the lower nibble of A is less than that of immdiate value
        ctx.regs.f |= FLAG_H;
    }
    if(ctx.regs.a < value){//C flag set if less for carry
        ctx.regs.f |= FLAG_C;
    }
    ctx.cycles += 8;
}