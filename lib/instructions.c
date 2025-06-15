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

    instruction_table[0x10] = stop_0;
    instruction_table[0x11] = ld_de_d16;
    instruction_table[0x12] = ld_de_a;
    instruction_table[0x13] = instr_inc_de;
    instruction_table[0x14] = instr_inc_d;
    instruction_table[0x15] = instr_dec_d;
    instruction_table[0x16] = instr_ld_d_d8;
    instruction_table[0x17] = instr_rla;
    instruction_table[0x18] = jr_r8;
    instruction_table[0x19] = add_hl_de;
    instruction_table[0x1A] = instr_ld_a_de;
    instruction_table[0x1B] = instr_dec_de;
    instruction_table[0x1C] = instr_inc_e;
    instruction_table[0x1D] = instr_dec_e;
    instruction_table[0x1E] = instr_ld_e_d8;
    instruction_table[0x1F] = instr_rra;

    instruction_table[0x20] = instr_nz_r8;
    instruction_table[0x21] = instr_ld_hl_d16;
    instruction_table[0x22] = instr_ld_hlplus_a;
    instruction_table[0x23] = instr_inc_hl;
    instruction_table[0x24] = instr_inc_h;
    instruction_table[0x25] = instr_dec_h;
    instruction_table[0x26] = instr_ld_h_d8;
    instruction_table[0x27] = instr_daa;
    instruction_table[0x28] = instr_jr_z_r8;
    instruction_table[0x29] = add_hl_hl;
    instruction_table[0x2A] = instr_ld_a_hlplus;
    instruction_table[0x2B] = instr_dec_hl;
    instruction_table[0x2C] = instr_inc_l;
    instruction_table[0x2D] = instr_dec_l;
    instruction_table[0x2E] = instr_ld_l_d8;
    instruction_table[0x2F] = instr_cpl;

    instruction_table[0x30] = instr_nc_r8;
    instruction_table[0x31] = instr_ld_sp_d16;
    instruction_table[0x32] = instr_ld_hlminus_a;
    instruction_table[0x33] = instr_inc_sp;
    instruction_table[0x34] = instr_inc_8bithl;
    instruction_table[0x35] = instr_dec_hl8bit;
    instruction_table[0x36] = instr_ld_hl_d8;
    instruction_table[0x37] = instr_scf;
    instruction_table[0x38] = instr_jr_c_r8;
    instruction_table[0x39] = instr_add_hl_sp;
    instruction_table[0x3A] = instr_ld_a_hlminus;
    instruction_table[0x3B] = instr_dec_sp;
    instruction_table[0x3C] = instr_inc_a;
    instruction_table[0x3D] = instr_dec_a;
    instruction_table[0x3E] = ld_a_d8;
    instruction_table[0x3F] = instr_ccf;

    instruction_table[0x40] = instr_ld_b_b;
    instruction_table[0x41] = instr_ld_b_c;
    instruction_table[0x42] = instr_ld_b_d;
    instruction_table[0x43] = instr_ld_b_e;
    instruction_table[0x44] = instr_ld_b_h;
    instruction_table[0x45] = instr_ld_b_l;
    instruction_table[0x46] = instr_ld_b_hl;
    instruction_table[0x47] = instr_ld_b_a;
    instruction_table[0x48] = instr_ld_c_b;
    instruction_table[0x49] = instr_ld_c_c;
    instruction_table[0x4A] = instr_ld_c_d;
    instruction_table[0x4B] = instr_ld_c_e;
    instruction_table[0x4C] = instr_ld_c_h;
    instruction_table[0x4D] = instr_ld_c_l;
    instruction_table[0x4E] = instr_ld_c_hl;
    instruction_table[0x4F] = instr_ld_c_a;

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
    ctx.regs.f &= ~(FLAG_Z | FLAG_H | FLAG_N); //clear flags z and h to see if we have to mod and n for all inc 

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

    //check flags
    ctx.regs.f = 0;
    if(result == 0){//if the result eq zero then set zero flag
        ctx.regs.f |= FLAG_Z;
    }

    ctx.regs.f |= FLAG_N;//set the subtract flag
    if((ctx.regs.c & 0X0F) == 0x00){//if the lower nibble was at 0 then when we subtract we will borrow from 4th bit
        ctx.regs.f |= FLAG_H;//set the half carry flag since we borrow from 4th bit
    }

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

//10
void stop_0(){ //system stop does not need to be implemeted
    bus_read(ctx.regs.pc++);
    ctx.cycles += 4;

}

//11
void ld_de_d16(){ //load 16 bit value onto DE
    u8 low = bus_read(ctx.regs.pc++);
    u8 high = bus_read(ctx.regs.pc++);

    ctx.regs.d = high;
    ctx.regs.e = low;

    ctx.cycles += 12;
}

//12
void ld_de_a(){//load 8 bit val at a to address @ de
    u16 address = (ctx.regs.d << 8) | ctx.regs.e;
    bus_write(address, ctx.regs.a);
    ctx.cycles += 8;
}

//13
void instr_inc_de(){//increment val at de by one
    u16 de = (ctx.regs.d << 8) | ctx.regs.e;
    de++;
    ctx.regs.d = de >> 8 & 0xFF;
    ctx.regs.e = de & 0xFF;

    ctx.cycles += 8;
}

//14
void instr_inc_d(){
    u8 result = ctx.regs.d + 1;
    ctx.regs.f &= ~(FLAG_Z | FLAG_H | FLAG_N); //clear flags z and h to see if we have to mod and n (subtract) for all inc 

    if(result == 0){
        ctx.regs.f |= FLAG_Z;
    }
    if ((ctx.regs.d & 0x0F) == 0x0F) {
        ctx.regs.f |= FLAG_H;
    }

    ctx.regs.d = result;
    ctx.cycles += 4;

}

//15
void instr_dec_d(){//decrement register d
    u8 result = ctx.regs.d - 1;
    u8 carry = ctx.regs.f & FLAG_C;//Preserve carry flag

    //check flags
    ctx.regs.f = 0;
    if(result == 0){//if the result eq zero then set zero flag
        ctx.regs.f |= FLAG_Z;
    }

    ctx.regs.f |= FLAG_N;//set the subtract flag
    if((ctx.regs.d & 0X0F) == 0x00){//if the lower nibble was at 0 then when we subtract we will borrow from 4th bit
        ctx.regs.f |= FLAG_H;//set the half carry flag since we borrow from 4th bit
    }

    ctx.regs.f |= carry;//reapply carry flag
    ctx.regs.d = result;
    ctx.cycles += 4;

}

//16
void instr_ld_d_d8(){//load 8bit val to d
    u8 value = bus_read(ctx.regs.pc++); //get val
    ctx.regs.d = value; //set reg d to val
    ctx.cycles += 8;

}

//17
void instr_rla() {
    u8 old_carry = (ctx.regs.f & FLAG_C) ? 1 : 0;
    u8 old_bit7 = (ctx.regs.a & 0x80) >> 7;

    ctx.regs.a = (ctx.regs.a << 1) | old_carry;

    // Set flags
    ctx.regs.f = 0; // Clear Z, N, H, C
    if (old_bit7) {
        ctx.regs.f |= FLAG_C;
    }

    ctx.cycles += 4;
}


//18
void jr_r8(){
    s8 offset = (s8) bus_read(ctx.regs.pc++); //its signed so we cast to signed 8 bit
    ctx.regs.pc += offset;
    ctx.cycles += 12;
}

//19
void add_hl_de(){ //add reg DE to HL
    u16 hl = (ctx.regs.h << 8) | ctx.regs.l;
    u16 de = (ctx.regs.d << 8) | ctx.regs.e;

    u32 result = hl + de;
    ctx.regs.f &= ~FLAG_N; //clear n flag(negate it)

    if(((hl & 0x0FFF) + (de & 0x0FFF)) > 0x0FFF){
        ctx.regs.f |= FLAG_H;
    }
    else{
        ctx.regs.f &= ~FLAG_H;
    }

    // Set C flag if carry from bit 15
    if (result > 0xFFFF) {
        ctx.regs.f |= FLAG_C;
    } 
    else {
        ctx.regs.f &= ~FLAG_C;
    }

    // Store result back in HL
    ctx.regs.h = (result >> 8) & 0xFF; //shift result to get just the high
    ctx.regs.l = result & 0xFF;

    ctx.cycles += 8;

}

//1A
void instr_ld_a_de(){ //load val at addy in de to a
    u16 address = (ctx.regs.d << 8) | ctx.regs.e;
    ctx.regs.a = bus_read(address);
    ctx.cycles += 8;
}

//1B
void instr_dec_de(){ //decrement de by one
    u16 de = (ctx.regs.d << 8) | ctx.regs.e;
    de--;
    ctx.regs.d = (de >> 8) & 0x0FF;
    ctx.regs.e = de & 0x0FF;
    ctx.cycles += 8;
}

//1C
void instr_inc_e(){
    u8 result = ctx.regs.e + 1;
    ctx.regs.f &= FLAG_C; // Preserve Carry flag only
    ctx.regs.f &= ~FLAG_N; //clear n flag

    if((ctx.regs.e & 0x0F) + 1 > 0x0F){
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

    ctx.regs.e = result;
    ctx.cycles += 4;
}

//1D
void instr_dec_e(){//decrement register e
    u8 result = ctx.regs.e - 1;

    //check flags
    ctx.regs.f = 0;
    if(result == 0){//if the result eq zero then set zero flag
        ctx.regs.f |= FLAG_Z;
    }

    ctx.regs.f |= FLAG_N;//set the subtract flag
    if((ctx.regs.e & 0X0F) == 0x00){//if the lower nibble was at 0 then when we subtract we will borrow from 4th bit
        ctx.regs.f |= FLAG_H;//set the half carry flag since we borrow from 4th bit
    }

    ctx.regs.e = result;
    ctx.cycles += 4;

}

//1E
void instr_ld_e_d8(){
    u8 value = bus_read(ctx.regs.pc++);
    ctx.regs.e = value;
    ctx.cycles += 8;

}

//1F
void instr_rra(){ //rotate a to the right and the last bit(bit 0) is moved to carry and carry to first bit(bit 7)
    u8 carryIn = (ctx.regs.f & FLAG_C) ?  0x80 : 0x00; //if carry is set then we make carry in 0x80 which equals 1000000 otherwise 0x00 which is 00000000
    u8 carryOut = ctx.regs.a & 0x01; //get the value we will be setting to the flag

    ctx.regs.a = (ctx.regs.a >> 1) | carryIn; //we shift the a reg and then or with carry in to make it the 7th bit

    //now we reset the flags since all are set to 0 except C
    ctx.regs.f = 0;
    if(carryOut){
        ctx.regs.f |= FLAG_C; //set bit 4 to 1 in the f regs
    }

    ctx.cycles += 4;
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

//22
void instr_ld_hlplus_a(){ //load whats on a to addy stored in HL and increment HL
    u16 address = (ctx.regs.h << 8) | ctx.regs.l; //combine regs to get addy
    bus_write(address, ctx.regs.a);

    address++;//increment HL
    ctx.regs.h = (address >> 8) & 0xFF;//take upper 8 bits of address and store in h. It does it by shifting the upper 8 bits down and then zeroing out the first 8 and u8 automatically takes the lower 8 bits
    ctx.regs.l = address & 0xFF; //take lower 8 bits

    ctx.cycles += 8;
}

//23
void instr_inc_hl(){//increment val at hl by one
    u16 hl = (ctx.regs.h << 8) | ctx.regs.l; //combine hl into one value
    hl++; //increment it
    ctx.regs.h = hl >> 8 & 0xFF; //set back the bits in upper then lower
    ctx.regs.l = hl & 0xFF;

    ctx.cycles += 8;
}

//24
void instr_inc_h(){ //increment val at h by one then check flags
    u8 result = ctx.regs.h + 1;
    ctx.regs.f &= ~(FLAG_Z | FLAG_H | FLAG_N); //clear flags z and h to see if we have to mod and n (subtract) for all inc 

    if(result == 0){
        ctx.regs.f |= FLAG_Z;
    }
    if ((ctx.regs.h & 0x0F) == 0x0F) {
        ctx.regs.f |= FLAG_H;
    }

    ctx.regs.h = result;
    ctx.cycles += 4;

}

//25
void instr_dec_h(){//decrement val at register h and check flags
    u8 result = ctx.regs.h - 1;
    u8 carry = ctx.regs.f & FLAG_C;//Preserve carry flag

    //check flags
    ctx.regs.f = 0;
    if(result == 0){//if the result eq zero then set zero flag
        ctx.regs.f |= FLAG_Z;
    }

    ctx.regs.f |= FLAG_N;//set the subtract flag
    if((ctx.regs.h & 0X0F) == 0x00){//if the lower nibble was at 0 then when we subtract we will borrow from 4th bit
        ctx.regs.f |= FLAG_H;//set the half carry flag since we borrow from 4th bit
    }

    ctx.regs.f |= carry;//reapply carry flag
    ctx.regs.h = result;
    ctx.cycles += 4;

}

//26
void instr_ld_h_d8(){//load 8bit val to h
    u8 value = bus_read(ctx.regs.pc++); //get val
    ctx.regs.h = value; //set reg h to val
    ctx.cycles += 8;

}

//27
void instr_daa(){//function used after performing arthmetic instructions whose inputs where binary, adjusting the result to likewise be in binary
    u8 correction = 0; //the correction/adjustment that will be ultimatley added to reg A
    bool set_carry = false; //will flag if we set carry or not

    //we have to check the N flag first to know what to do(according to https://rgbds.gbdev.io/docs/v0.9.2/gbz80.7#DAA)
    //the subtract flag tells us if we are executing this function after a sub or add fucntion

    //if N is not set(ADD)
    if(!(ctx.regs.f & FLAG_N)){
        //check half carry
        if((ctx.regs.f & FLAG_H) || (ctx.regs.a & 0x0F) > 0x09){
            correction |= 0x06;
        }
        
        //check carry flag
        if((ctx.regs.f & FLAG_C) || (ctx.regs.a > 0x99)){
            correction |= 0x60;
            set_carry = true;
        }
        ctx.regs.a += correction; //finally add the correction to reg A
    }
    //else means thatt the prev opcode was SUB
    else{
        //check half carry
        if(ctx.regs.f & FLAG_H){
            correction |= 0x06;
        }

        //check carry
        if(ctx.regs.f & FLAG_C){
            correction |= 0x60;
        }
        ctx.regs.a -= correction;
    }

    //set flags
    ctx.regs.f &= ~(FLAG_Z | FLAG_H); //clear z and h since we modify them (h is always 0)
    if(ctx.regs.a == 0){ //check for Z flag
        ctx.regs.f |= FLAG_Z;
    }
    if(set_carry){ //check for C flag 
        ctx.regs.f |= FLAG_C;
    }

    ctx.cycles += 4;

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

//29
void add_hl_hl(){ //adds 16 bit value in HL reg pair to itself
    u16 hl = (ctx.regs.h << 8) | ctx.regs.l;

    u32 result = hl + hl;
    ctx.regs.f &= ~FLAG_N; //clear n flag(negate it)

    if(((hl & 0x0FFF) + (hl & 0x0FFF)) > 0x0FFF){
        ctx.regs.f |= FLAG_H;
    }
    else{
        ctx.regs.f &= ~FLAG_H;
    }

    // Set C flag if carry from bit 15
    if (result > 0xFFFF) {
        ctx.regs.f |= FLAG_C;
    } 
    else {
        ctx.regs.f &= ~FLAG_C;
    }

    // Store result back in HL
    ctx.regs.h = (result >> 8) & 0xFF; //shift result to get just the high
    ctx.regs.l = result & 0xFF;

    ctx.cycles += 8;

}

//2A
void instr_ld_a_hlplus(){ //load val at addy in hl to a then increment hl
    u16 address = (ctx.regs.h << 8) | ctx.regs.l;
    ctx.regs.a = bus_read(address);

    address++; //increment addy then load it back
    ctx.regs.h = (address >> 8) & 0xFF; //push the leftmost 4 bytes down like so 0000 1234 then only take the last 4 with 0xFF
    ctx.regs.l = address & 0xFF; //just take the last 4 like so 1234 [5678]

    ctx.cycles += 8;
}

//2B
void instr_dec_hl(){ //decrement hl by one
    u16 hl = (ctx.regs.h << 8) | ctx.regs.l;
    hl--;
    ctx.regs.h = (hl >> 8) & 0x0FF;
    ctx.regs.l = hl & 0x0FF;
    ctx.cycles += 8;
}

//2C
void instr_inc_l(){ //increment 8 bit val stored at l
    u8 result = ctx.regs.l + 1;
    ctx.regs.f &= FLAG_C; // Preserve Carry flag only
    ctx.regs.f &= ~FLAG_N; //clear n flag

    if((ctx.regs.l & 0x0F) + 1 > 0x0F){
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

    ctx.regs.l = result;
    ctx.cycles += 4;
}

//2D
void instr_dec_l(){//decrement 8-bit register l
    u8 result = ctx.regs.l - 1;

    //check flags
    ctx.regs.f = 0;
    if(result == 0){//if the result eq zero then set zero flag
        ctx.regs.f |= FLAG_Z;
    }

    ctx.regs.f |= FLAG_N;//set the subtract flag
    if((ctx.regs.l & 0X0F) == 0x00){//if the lower nibble was at 0 then when we subtract we will borrow from 4th bit
        ctx.regs.f |= FLAG_H;//set the half carry flag since we borrow from 4th bit
    }

    ctx.regs.l = result;
    ctx.cycles += 4;

}

//2E
void instr_ld_l_d8(){ //8-bit load instr. Load 8bit val to l
    u8 value = bus_read(ctx.regs.pc++);
    ctx.regs.l = value;
    ctx.cycles += 8;

}

//2F
void instr_cpl(){ //complement acucumulator for a so -> a = ~a also set flags n and h
    ctx.regs.a = ~ctx.regs.a;

    //set flags
    ctx.regs.f |= FLAG_N | FLAG_H;

    ctx.cycles += 4;
}

//30
void instr_nc_r8(){    //jump if c flag is NOT set
    if(!(ctx.regs.f & FLAG_C)){//if flag c is not set then jump
        s8 offset = (s8) bus_read(ctx.regs.pc++); //cast to signed char so that neg offset works
        ctx.regs.pc += offset;
        ctx.cycles += 12;
    }
    else{
        ctx.regs.pc++;
        ctx.cycles += 8;
    }
}

//31
void instr_ld_sp_d16(){
    //load 16 bit immediate to sp
    u8 low = bus_read(ctx.regs.pc++);
    u8 high = bus_read(ctx.regs.pc++);

    ctx.regs.sp = (high << 8) | low; //combine into sp
    ctx.cycles += 12;

}

//32
void instr_ld_hlminus_a(){ //load whats on a to addy stored in HL and decrement HL
    u16 address = (ctx.regs.h << 8) | ctx.regs.l; //combine regs to get addy
    bus_write(address, ctx.regs.a);

    address--;//decrement HL
    ctx.regs.h = (address >> 8) & 0xFF;//take upper 8 bits of address and store in h. It does it by shifting the upper 8 bits down and then zeroing out the first 8 and u8 automatically takes the lower 8 bits
    ctx.regs.l = address & 0xFF; //take lower 8 bits

    ctx.cycles += 8;
}

//33
void instr_inc_sp(){//increment val at sp by one
    ctx.regs.sp++; //increment it
    ctx.cycles += 8;
}

//34
void instr_inc_8bithl(){//increment val at addy hl then check flags
    u16 address = (ctx.regs.h << 8) | ctx.regs.l; // Form HL
    u8 value = bus_read(address);                // Read byte from memory
    u8 result = value + 1;                       // Increment the value

    // Preserve carry flag only
    ctx.regs.f &= FLAG_C;
    ctx.regs.f &= ~FLAG_N; // Clear N flag (this is not a subtraction)

    if ((value & 0x0F) == 0x0F) {
        ctx.regs.f |= FLAG_H; // Set H if lower nibble overflows
    } else {
        ctx.regs.f &= ~FLAG_H;
    }

    if (result == 0) {
        ctx.regs.f |= FLAG_Z;
    } else {
        ctx.regs.f &= ~FLAG_Z;
    }

    bus_write(address, result);                 // Write incremented value back to memory
    ctx.cycles += 12;
}

//35
void instr_dec_hl8bit() {//decrement val at addy hl then check flags
    u16 address = (ctx.regs.h << 8) | ctx.regs.l; // Form HL
    u8 value = bus_read(address);                // Read byte from memory
    u8 result = value - 1;                       // decrement the value

    ctx.regs.f &= FLAG_C; // Preserve carry flag only
    ctx.regs.f |= FLAG_N; // set N flag

    if ((value & 0x0F) == 0x00) {
        ctx.regs.f |= FLAG_H; // Set H if borrow from bit 4
    } else {
        ctx.regs.f &= ~FLAG_H;
    }

    if (result == 0) {
        ctx.regs.f |= FLAG_Z;
    } else {
        ctx.regs.f &= ~FLAG_Z;
    }

    bus_write(address, result);                 // Write decrement value back to memory
    ctx.cycles += 12;
}

//36
void instr_ld_hl_d8(){//load 8bit val to hl address
    u8 value = bus_read(ctx.regs.pc++); //get val
    u16 address = (ctx.regs.h << 8) | ctx.regs.l;

    bus_write(address, value);

    ctx.cycles += 12;
}

//37
void instr_scf(){ //just sets the carry flag and sets 0 for H and N
    ctx.regs.f &= ~(FLAG_H | FLAG_N); //set 0 to H and N
    ctx.regs.f |= FLAG_C; //set c flag

    ctx.cycles += 4;
}

//38
void instr_jr_c_r8(){ //jump if c flag is set

    if(ctx.regs.f & FLAG_C){//if flag c is set
        s8 offset = (s8) bus_read(ctx.regs.pc++); //cast to signed char so that neg offset works
        ctx.regs.pc += offset;
        ctx.cycles += 12;
    }
    else{
        ctx.regs.pc++;
        ctx.cycles += 8;
    }
}

//39
void instr_add_hl_sp(){ //adds 16 bit value in SP reg pair to HL
    u16 hl = (ctx.regs.h << 8) | ctx.regs.l;

    u32 result = hl + ctx.regs.sp;
    ctx.regs.f &= ~FLAG_N; //clear n flag(negate it)

    if(((hl & 0x0FFF) + (ctx.regs.sp & 0x0FFF)) > 0x0FFF){
        ctx.regs.f |= FLAG_H;
    }
    else{
        ctx.regs.f &= ~FLAG_H;
    }

    // Set C flag if carry from bit 15
    if (result > 0xFFFF) {
        ctx.regs.f |= FLAG_C;
    } 
    else {
        ctx.regs.f &= ~FLAG_C;
    }

    // Store result back in HL
    ctx.regs.h = (result >> 8) & 0xFF;
    ctx.regs.l = result & 0xFF;

    ctx.cycles += 8;

}

//3A
void instr_ld_a_hlminus(){ //load val at addy in hl to reg a then decrement hl
    u16 address = (ctx.regs.h << 8) | ctx.regs.l;
    ctx.regs.a = bus_read(address);

    address--; //decrement addy then load it back
    ctx.regs.h = (address >> 8) & 0xFF; //push the leftmost 4 bytes down like so 0000 1234 then only take the last 4 with 0xFF
    ctx.regs.l = address & 0xFF; //just take the last 4 like so 1234 [5678]

    ctx.cycles += 8;
}

//3B
void instr_dec_sp(){ //decrement sp by one
    ctx.regs.sp--;
    ctx.cycles += 8;
}

//3C
void instr_inc_a(){ //increment 8 bit val stored at a
    u8 result = ctx.regs.a + 1;
    ctx.regs.f &= FLAG_C; // Preserve Carry flag only
    ctx.regs.f &= ~FLAG_N; //clear n flag

    if((ctx.regs.a & 0x0F) + 1 > 0x0F){
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

    ctx.regs.a = result;
    ctx.cycles += 4;
}

//3D
void instr_dec_a(){//decrement 8-bit register a
    u8 result = ctx.regs.a - 1;

    //check flags
    ctx.regs.f = 0;
    if(result == 0){//if the result eq zero then set zero flag
        ctx.regs.f |= FLAG_Z;
    }

    ctx.regs.f |= FLAG_N;//set the subtract flag
    if((ctx.regs.a & 0X0F) == 0x00){//if the lower nibble was at 0 then when we subtract we will borrow from 4th bit
        ctx.regs.f |= FLAG_H;//set the half carry flag since we borrow from 4th bit
    }

    ctx.regs.a = result;
    ctx.cycles += 4;

}

//3E
void ld_a_d8(){
    u8 value = bus_read(ctx.regs.pc++);
    ctx.regs.a = value;
    ctx.cycles += 8;
}

//3F
void instr_ccf(){//set n and h flags to zero and invert c flag
    ctx.regs.f &= ~FLAG_N;
    ctx.regs.f &= ~FLAG_H;
    
    // Invert the Carry flag (if it was 1, clear it; if it was 0, set it)
    if (ctx.regs.f & FLAG_C) {
        ctx.regs.f &= ~FLAG_C; // Clear carry
    } else {
        ctx.regs.f |= FLAG_C;  // Set carry
    }

    ctx.cycles += 4;
}

//40
void instr_ld_b_b(){//no op since we are to load reg val b to b
    ctx.cycles += 4;
}

//41
void instr_ld_b_c(){//load reg c vals to b
    ctx.regs.b = ctx.regs.c;
    ctx.cycles += 4;
}

//42
void instr_ld_b_d(){//load reg d vals to b
    ctx.regs.b = ctx.regs.d;
    ctx.cycles += 4;
}

//43
void instr_ld_b_e(){
    ctx.regs.b = ctx.regs.e;
    ctx.cycles += 4;
}

//44
void instr_ld_b_h(){
    ctx.regs.b = ctx.regs.h;
    ctx.cycles += 4;
}

//45
void instr_ld_b_l(){
    ctx.regs.b = ctx.regs.l;
    ctx.cycles += 4;
}

//46
void instr_ld_b_hl(){
    u16 address = (ctx.regs.h << 8) | ctx.regs.l;
    ctx.regs.b = bus_read(address);
    ctx.cycles += 8;
}

//47
void instr_ld_b_a(){
    ctx.regs.b = ctx.regs.a;
    ctx.cycles += 4;
}

//48
void instr_ld_c_b(){
    ctx.regs.c = ctx.regs.b;
    ctx.cycles += 4;
}

//49
void instr_ld_c_c(){
    ctx.cycles += 4;
}

//4A
void instr_ld_c_d(){
    ctx.regs.c = ctx.regs.d;
    ctx.cycles += 4;
}

//4B
void instr_ld_c_e(){
    ctx.regs.c = ctx.regs.e;
    ctx.cycles += 4;
}

//4C
void instr_ld_c_h(){
    ctx.regs.c = ctx.regs.h;
    ctx.cycles += 4;
}

//4D
void instr_ld_c_l(){
    ctx.regs.c = ctx.regs.l;
    ctx.cycles += 4;
}

//4E
void instr_ld_c_hl(){
    u16 address = (ctx.regs.h << 8) | ctx.regs.l;
    ctx.regs.c = bus_read(address);
    ctx.cycles += 8;
}

//4F
void instr_ld_c_a(){
    ctx.regs.c = ctx.regs.a;
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