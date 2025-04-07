#include <instructions.h>
#include <cpu.h>
#include <bus.h>
#include <stack.h>

extern cpu_context ctx;

InstructionFunc instruction_table[256] = {0};//declare the table 
InstructionFuncCB instruction_tableCB[256] = {0};//declare the CB table 

//This function will initialize the table and add the opcodes to the correct address
void init_instruction_table(){
    instruction_table[0x00] = instr_nop;
    instruction_table[0x01] = ld_bc_d16;
    instruction_table[0x02] = instr_ld_bc_a;
    instruction_table[0x03] = inc_bc;
    instruction_table[0x04] = inc_b;
    instruction_table[0x05] = instr_dec_b;
    instruction_table[0x06] = instr_ld_b_d8;
    instruction_table[0x07] = instr_rlca;
    instruction_table[0x08] = instr_ld_a16_sp;
    instruction_table[0x09] = instr_add_hl_bc;
    instruction_table[0x0A] = instr_ld_a_bc;
    instruction_table[0x0B] = instr_dec_bc;
    instruction_table[0x0C] = instr_inc_c;
    instruction_table[0x0D] = instr_dec_c;
    instruction_table[0x0E] = instr_ld_c_d8;
    instruction_table[0x0F] = instr_rrca;

    instruction_table[0x18] = jr_r8;

    instruction_table[0x20] = instr_nz_r8;
    instruction_table[0x21] = instr_ld_hl_d16;
    instruction_table[0x28] = instr_jr_z_r8;

    instruction_table[0x32] = instr_ld_hl_a;
    instruction_table[0x3E] = ld_a_d8;

    instruction_table[0x47] = instr_ld_b_a;

    instruction_table[0x6F] = instr_ld_la;

    instruction_table[0xAF] = instr_xor_a;

    instruction_table[0xC3] = instr_jp_a16;
    instruction_table[0xCB] = instr_prefix_cb;
    instruction_table[0xCD] = insr_call_a16;

    instruction_table[0xE0] = instr_ldh_a8_a;
    instruction_table[0xEA] = instr_ld_a16_a;
    instruction_table[0xF0] = instr_ldh_a_a8;
    instruction_table[0xF3] = instr_di;
    instruction_table[0xFA] = instr_ld_a_a16;
    instruction_table[0xFE] = instr_cp_d8;
}

//Just like above but for CB opcodes
void init_instruction_tableCB(){
    instruction_tableCB[0x87] = cb_instr_res_0_a;
}

//actual regular opcode function definitions below:


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

//02
void instr_ld_bc_a(){//load a onto mem address at BC
    u16 address = (ctx.regs.b << 8) | ctx.regs.c; //shift high and or with low
    bus_write(address, ctx.regs.a);
    ctx.cycles += 8;

}

//03
void inc_bc(){//increment the value stored at bc
    u16 bc = (ctx.regs.b << 8) | ctx.regs.c;//put together, increment, then split
    bc++;
    ctx.regs.b = (bc >> 8) & 0xFF;
    ctx.regs.c = bc & 0xFF;
    ctx.cycles += 8;

}

//04
void inc_b(){
    u8 result = ctx.regs.b + 1;
    ctx.regs.f &= ~(FLAG_Z | FLAG_H); // Clear Z and H flags

    if(result == 0){
        ctx.regs.f |= FLAG_Z;
    }
    if ((ctx.regs.b & 0x0F) == 0x0F) {
        ctx.regs.f |= FLAG_H;
    }

    ctx.regs.b = result;
    ctx.cycles += 4;

}

//05
void instr_dec_b(){//decrement register b
    u8 result = ctx.regs.b - 1;
    u8 carry = ctx.regs.f & FLAG_C;//Preserve carry flag

    //check flags
    ctx.regs.f = 0;
    if(result == 0){//if the result eq zero then set zero flag
        ctx.regs.f |= FLAG_Z;
    }

    ctx.regs.f |= FLAG_N;//set the subtract flag
    if((ctx.regs.b & 0X0F) == 0x00){//if the lower nibble was at 0 then when we subtract we will borrow from 4th bit
        ctx.regs.f |= FLAG_H;//set the half carry flag since we borrow from 4th bit
    }

    ctx.regs.f |= carry;//reapply carry flag
    ctx.regs.b = result;
    ctx.cycles += 4;

}

//06
void instr_ld_b_d8(){
    u8 value = bus_read(ctx.regs.pc++);
    ctx.regs.b = value;
    ctx.cycles += 8;

}

//07
void instr_rlca(){//rotate a left. old bit 7 to carry flag and bit 0.
    u8 a = ctx.regs.a;
    u8 msb = (a & 0x80) >> 7; //get most sig bit

    ctx.regs.a = (a << 1) | msb; //rotate left through msb into lsb
    ctx.regs.f = 0;
    if(msb){
        ctx.regs.f |= FLAG_C;
    }

    ctx.cycles += 4;
}


//08
void instr_ld_a16_sp(){ //store 16 bit val of the sp into the 16 bit address a16
    u8 low = bus_read(ctx.regs.pc++);
    u8 high = bus_read(ctx.regs.pc++);
    u16 address = (high << 8) | low;

    bus_write(address, ctx.regs.sp & 0xFF); //write the low
    bus_write(address + 1, (ctx.regs.sp >> 8)); //write the high

    ctx.cycles += 20;
}

//09
void instr_add_hl_bc(){ //add hl and bc and store to hl
    u16 hl = (ctx.regs.h << 8) | ctx.regs.l;
    u16 bc = (ctx.regs.b << 8) | ctx.regs.c;

    u32 result = bc + hl; //32 to detect carry
    ctx.regs.f &= ~(FLAG_N | FLAG_H | FLAG_C); //clear n h and c

    if(((hl & 0x0FFF) + (bc & 0x0FFF)) > 0x0FFF){ //check for half carry from bit 11 since 32-bit
        ctx.regs.f |= FLAG_H;
    }

    if(result > 0xFFFF){ //check for full carry bit 15
        ctx.regs.f |= FLAG_C;
    }

    ctx.regs.h = (result >> 8) & 0xFF;
    ctx.regs.l = result & 0xFF;
    ctx.cycles += 8;
}

//0A
void instr_ld_a_bc(){ //load val at addy bc to a
    u16 address = (ctx.regs.b << 8) | ctx.regs.c;
    ctx.regs.a = bus_read(address);
    ctx.cycles += 8;
}

//0B
void instr_dec_bc(){ //decrement bc by one
    u16 bc = (ctx.regs.b << 8) | ctx.regs.c;
    bc--;
    ctx.regs.b = (bc >> 8) & 0x0FF;
    ctx.regs.c = bc & 0x0FF;
    ctx.cycles += 8;
}

//0C
void instr_inc_c(){
    u8 result = ctx.regs.c + 1;
    ctx.regs.f &= FLAG_C; // Preserve Carry flag only
    ctx.regs.f &= ~FLAG_N; //clear n flag

    if((ctx.regs.c & 0x0F) == 0x0F){
        ctx.regs.f |= FLAG_H; //set half-carry if lower nibble was 0xF
    }
    else{
        ctx.regs.f &= ~FLAG_H;
    }

    if(result == 0){
        ctx.regs.f |= FLAG_Z;
    }
    else{
        ctx.regs.f &= ~FLAG_Z;
    }

    ctx.regs.c = result;
    ctx.cycles += 4;
}

//0D
void instr_dec_c(){//decrement register c
    u8 result = ctx.regs.c - 1;
    u8 carry = ctx.regs.f & FLAG_C;//Preserve carry flag

    //check flags
    ctx.regs.f = 0;
    if(result == 0){//if the result eq zero then set zero flag
        ctx.regs.f |= FLAG_Z;
    }

    ctx.regs.f |= FLAG_N;//set the subtract flag
    if((ctx.regs.c & 0X0F) == 0x00){//if the lower nibble was at 0 then when we subtract we will borrow from 4th bit
        ctx.regs.f |= FLAG_H;//set the half carry flag since we borrow from 4th bit
    }

    ctx.regs.f |= carry;//reapply carry flag
    ctx.regs.c = result;
    ctx.cycles += 4;

}

//0E
void instr_ld_c_d8(){
    u8 value = bus_read(ctx.regs.pc++);
    ctx.regs.c = value;
    ctx.cycles += 8;

}

//0F
void instr_rrca(){ //rotate the contents of reg a right by 1 bit
    u8 a = ctx.regs.a;
    u8 bit0 = a & 0x01;

    ctx.regs.a = (a >> 1) | (bit0 << 7); //rotate right, wrap bit 0 to bit 7

    ctx.regs.f = 0;
    if(bit0){
        ctx.regs.f &= FLAG_C; //set carry flag if we had the bit before
    }

    ctx.cycles += 4;
}

//18
void jr_r8(){
    s8 offset = (s8) bus_read(ctx.regs.pc++); //its signed so we cast to signed 8 bit
    ctx.regs.pc += offset;
    ctx.cycles += 12;
}   

//20
void instr_nz_r8(){
    //jump if z flag is NOT set
    if(!(ctx.regs.f & FLAG_Z)){//if flag z is set
        s8 offset = (s8) bus_read(ctx.regs.pc++); //cast to signed char so that neg offset works
        ctx.regs.pc += offset;
        ctx.cycles += 12;
    }
    else{
        ctx.regs.pc++;
        ctx.cycles += 8;
    }
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

//32
void instr_ld_hl_a(){//load onto the address in HL whats on A then decrement HL
    u16 address = (ctx.regs.h << 8) | ctx.regs.l; //combine h and l to HL
    bus_write(address, ctx.regs.a);//@address write whats on a

    address--;
    ctx.regs.h = (address >> 8) & 0xFF;//store updated high byte
    ctx.regs.l = address & 0XFF;//store the updated low
    ctx.cycles += 8;
}

//3E
void ld_a_d8(){
    u8 value = bus_read(ctx.regs.pc++);
    ctx.regs.a = value;
    ctx.cycles += 8;
}

//47
void instr_ld_b_a(){
    ctx.regs.b = ctx.regs.a;
    ctx.cycles += 4;
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

//CB
void instr_prefix_cb(){//calls whatever CB opcode is at the next pc
    u8 cb_opcode = bus_read(ctx.regs.pc++); //pc contained the CB opcode so we take it and increment
    //if we find the instruction then execute it
    if(instruction_tableCB[cb_opcode]){
        printf("***Executing CB opcode %02X\n\n", cb_opcode);
        instruction_tableCB[cb_opcode]();//then call the actual function
    }
    else{
        printf("*********Instruction %02X not yet implemented FOR CB********\n\n", cb_opcode);
    }
}

//CD
void insr_call_a16(){
    u8 low = bus_read(ctx.regs.pc++);
    u8 high = bus_read(ctx.regs.pc++);
    u16 address = (high << 8) | low;//this is the address we want to go to do the subroutine

    u16 return_address = ctx.regs.pc; //collect the current pc to return to
    stack_push16(return_address);//push return addy to the stack so we know where to come back

    ctx.regs.pc = address;//set the pc to the new address and go off in the distance
    ctx.cycles += 24;
}


//E0
void instr_ldh_a8_a(){
    u8 offset = bus_read(ctx.regs.pc++);//get offset
    u16 address = 0xFF00 + offset;//offset by this much
    bus_write(address, ctx.regs.a);
    ctx.cycles += 12;
}

//EA
void instr_ld_a16_a(){//load onto 16 bit address whats in A
    u8 low = bus_read(ctx.regs.pc++);
    u8 high = bus_read(ctx.regs.pc++);
    u16 address = (high << 8) | low;

    bus_write(address, ctx.regs.a);
    ctx.cycles += 16;
}

//F0
void instr_ldh_a_a8(){
    u8 offset = bus_read(ctx.regs.pc++);
    u16 address = 0xFF00 + offset;
    ctx.regs.a = bus_read(address);
    ctx.cycles += 12;
}

//F3
void instr_di(){
    ctx.ime = false;
    ctx.cycles += 4;
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

//CB instruction implementations go here:

//87
void cb_instr_res_0_a(){
    ctx.regs.a &= ~(1 << 0);
    ctx.cycles += 8;
}