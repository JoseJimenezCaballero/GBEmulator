#include <cpu.h>
#include <bus.h>
#include <instructions.h>

cpu_context ctx = {0};

void cpu_init(){
    ctx.regs.pc = 0x100; //make the pc the entry point
    ctx.regs.sp = 0xFFFE; //make stack point to the top of HRAM(fastest for cpu)
    init_instruction_table();//init instruction tables
    init_instruction_tableCB();
}

cpu_registers *cpu_get_regs(){ //getter to get the regs which we will use here and there
    return &ctx.regs;
}

static void fetch_instruction(){
    ctx.opcode = bus_read(ctx.regs.pc++); //read from rom data at address pc then increment pc by one
    //now that we have instructions we need to fetch the fucntion that will execute this and return it
}
static void execute(u16 opcode){
    //here we will run the function needed for the cpu and then return so we 
    //will need to get the function and execute it

    //if we find the instruction then execute it
    if(instruction_table[opcode]){
        instruction_table[opcode]();//then call the actual function
    }
    else{
        printf("Instruction %02X not yet implemented\n", opcode);
        exit(-1);
    }

}

//every step will check if the cpu is halted if not it will fetch instruction and execute
bool cpu_step(){
    if(!ctx.halted){
        u16 pc = ctx.regs.pc;
        fetch_instruction();
        printf("Executing Instruction: %02X      Num of cycles: %016X\nRegisters: A:%02X   B:%02X   C:%02X   D:%02X   E:%02X   F:%02X   H:%02X   L:%02X   PC: %04X   SP: %04X\n\n", ctx.opcode, ctx.cycles, ctx.regs.a, ctx.regs.b,ctx.regs.c,ctx.regs.d,ctx.regs.e,ctx.regs.f,ctx.regs.h,ctx.regs.l, pc, ctx.regs.sp);
        execute(ctx.opcode);
    }
    return true;
}