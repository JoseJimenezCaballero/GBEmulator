#include <instructions.h>
#include <cpu.h>
#include <bus.h>

extern cpu_context ctx;

InstructionFunc instruction_table[256] = {0};//declare the table 

//This function will initialize the table and add the opcodes to the correct address
void init_instruction_table(){
    instruction_table[0x00] = instr_nop;
    instruction_table[0x01] = ld_bc_d16;
    instruction_table[0x06] = instr_ld_b_d8;
    instruction_table[0x0E] = instr_ld_c_d8;

    instruction_table[0x18] = jr_r8;

    instruction_table[0x21] = instr_ld_hl_d16;
    instruction_table[0x28] = instr_jr_z_r8;

    instruction_table[0x6F] = instr_ld_la;

    instruction_table[0xAF] = instr_xor_a;

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

//01
void ld_bc_d16(){
    //load 16 bit immediate
    u8 low = bus_read(ctx.regs.pc++);
    u8 high = bus_read(ctx.regs.pc++);
    ctx.regs.b = high;
    ctx.regs.c = low;
    ctx.cycles += 12;
}


//06
void instr_ld_b_d8(){
    u8 value = bus_read(ctx.regs.pc++);
    ctx.regs.b = value;
    ctx.cycles += 8;

}

//18
void jr_r8(){
    s8 offset = (s8) bus_read(ctx.regs.pc++); //its signed so we cast to signed 8 bit
    ctx.regs.pc += offset;
    ctx.cycles += 12;
}   

//28
void instr_jr_z_r8(){
    //jump if z flag is set
    if(ctx.regs.f & FLAG_Z){//if flag z is set
        s8 offset = (s8) bus_read(ctx.regs.pc++); //cast to signed char so that neg offset works
        ctx.regs.pc += offset;
        ctx.cycles += 12;
    }
    else{
        ctx.regs.pc++;
        ctx.cycles += 8;
    }
}

//0E
void instr_ld_c_d8(){
    u8 value = bus_read(ctx.regs.pc++);
    ctx.regs.c = value;
    ctx.cycles += 8;

}

//21
void instr_ld_hl_d16(){
    //load 16 bit immediate to registers
    u8 low = bus_read(ctx.regs.pc++);
    u8 high = bus_read(ctx.regs.pc++);
    ctx.regs.h = high;
    ctx.regs.l = low;
    ctx.cycles += 12;

}

//6F
void instr_ld_la(){
    ctx.regs.l = ctx.regs.a;
    ctx.cycles += 4;
}

//AF
void instr_xor_a(){
    //xor a with a which means set a to 0
    ctx.regs.a = 0;
    ctx.regs.f = 0;//clear flags like it says in opcode chart
    ctx.regs.f |= FLAG_Z;
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