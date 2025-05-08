#pragma once

#include "common.h"

//seperate file to deal with these read and write to ram blocks in the cart so as to not
//make the bus.c confusing

u8 wram_read(u16 address);
void wram_write(u16 address, u8 value);

u8 hram_read(u16 address);
void hram_write(u16 address, u8 value);