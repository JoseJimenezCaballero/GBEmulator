#pragma once

#include "common.h"

typedef struct {//we will read and write the header info for the cart into a struct 
    u8 entry[4]; //the entry point of the cpu to run instructions
    u8 logo[0x30];//the nintendo logo

    char title[16];//the upper case ascii title of the game
    u16 new_lic_code;//license code
    u8 sgb_flag;
    u8 type;//there are different types of cartridges
    u8 rom_size;//size of rom
    u8 ram_size;
    u8 dest_code;//to show if japan or not
    u8 lic_code;
    u8 version;
    u8 checksum;//checks if it is a valid rom file(use algorithm)
    u16 global_checksum;
} rom_header;

bool cart_load(char *cart);