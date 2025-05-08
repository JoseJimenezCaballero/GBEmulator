#include <ram.h>

/*
we emulate the internal rams by simply using an array. We must subtract the offset
everytime we receive an address bc our wram and hram start at 0 not at the given offset
*/
typedef struct {
    u8 wram[0x2000];
    u8 hram[0x80];
} ram_context;

static ram_context ctx;

u8 wram_read(u16 address){
    address -= 0xC000; //subtract this offset
    return ctx.wram[address];
}
void wram_write(u16 address, u8 value){
    address -= 0xC000; //subtract this offset
    ctx.wram[address] = value; //set the value at that addy
}

u8 hram_read(u16 address){
    address -= 0xFF80; //subtract this offset
    return ctx.hram[address];
}
void hram_write(u16 address, u8 value){
    address -= 0xFF80; //subtract this offset
    ctx.hram[address] = value; //set the value at that addy
}