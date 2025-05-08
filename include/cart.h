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

typedef struct { //the current cartridge being used will be kept in a context
    char filename[1024];//name
    u32 rom_size;//size
    u8 *rom_data;//its actual data will be loaded into memory
    rom_header *header;//header info will be contained here too
} cart_context;

//Cart load is used for initially loading the cartridge onto memory and making a struct for it and casting the data accordignly for headers and whatnot
bool cart_load(char *cart);

//Used for reading data from rom data at a particular address 
u8 cart_read(u16 address);

//Used for writing data from rom data at a particular address 
u8 cart_write(u16 address, u8 value);