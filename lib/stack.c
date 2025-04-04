#include <stack.h>
#include <cpu.h>
#include <bus.h>

void stack_push(u8 data){
    cpu_get_regs()->sp--;//get location of stack pointer and go lower
    bus_write(cpu_get_regs()->sp, data);//write onto sp the data and go lower
}

void stack_push16(u16 data){
    stack_push((data >> 8) & 0xFF); //operation for the high
    stack_push(data & 0xFF); //operation for the high
}

u8 stack_pop(){
    return bus_read(cpu_get_regs()->sp++); //move sp up so decrement it
}

u16 stack_pop16(){
    u8 low = stack_pop();
    u8 high = stack_pop();

    return (high << 8) | low;
}