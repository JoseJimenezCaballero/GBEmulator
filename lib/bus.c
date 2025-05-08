#include <bus.h>
#include <cart.h>
#include <ram.h>


//Bus will be used for all the reading and writing from the cartridge(which is now in memory)

//==========================
// Game Boy Memory Map for Cartdrige AND Internal Components
//==========================

// 0x0000 - 0x3FFF : ROM Bank 0
// - Owner     : Cartridge (ROM file)
// - Description: The first 16KB of the game ROM.
//                Always visible; usually contains interrupt vectors, header, and boot code.

// 0x4000 - 0x7FFF : ROM Bank N (Switchable via MBC)
// - Owner     : Cartridge (ROM file)
// - Description: Switchable ROM bank area (via Memory Bank Controller).
//                Used for games larger than 32KB.

// 0x8000 - 0x97FF : VRAM (CHR RAM / Tile Data)
// - Owner     : Game Boy PPU (internal GPU)
// - Description: Stores tile data for backgrounds and sprites.
//                Can be accessed by CPU when LCD is off or not rendering.

// 0x9800 - 0x9BFF : BG Tile Map 1
// - Owner     : Game Boy PPU
// - Description: Background map used to layout tiles on screen.
//                Each byte is a tile index.

// 0x9C00 - 0x9FFF : BG Tile Map 2
// - Owner     : Game Boy PPU
// - Description: Alternate tile map. The game can choose between these two maps for scrolling backgrounds.

// 0xA000 - 0xBFFF : External RAM (Save RAM)
// - Owner     : Cartridge (if it has save RAM and battery)
// - Description: Used to store saved game data (e.g., Pokémon saves).
//                Accessed via MBC if present.

// 0xC000 - 0xCFFF : WRAM Bank 0
// - Owner     : Game Boy CPU (internal RAM)
// - Description: First 4KB of work RAM. Always accessible.
//                Used for stack, variables, and game logic.

// 0xD000 - 0xDFFF : WRAM Bank 1–7 (CGB only)
// - Owner     : Game Boy CPU (CGB)
// - Description: Second 4KB of work RAM. On Color Game Boy, this region is bank switchable.
//                On DMG, it's fixed as one extra 4KB.

// 0xE000 - 0xFDFF : Echo RAM (mirror of 0xC000–0xDDFF)
// - Owner     : Game Boy CPU (not used)
// - Description: Mirrors WRAM but should not be used.
//                Kept for hardware compatibility. Writes should affect WRAM too (optional).

// 0xFE00 - 0xFE9F : OAM (Object Attribute Memory)
// - Owner     : Game Boy PPU
// - Description: Stores sprite attributes (position, tile, palette info).
//                Up to 40 sprites. Each uses 4 bytes.

// 0xFEA0 - 0xFEFF : Unusable
// - Owner     : Game Boy hardware
// - Description: Not usable. Reads return 0xFF, writes are ignored.
//                Reserved by hardware and should be skipped.

// 0xFF00 - 0xFF7F : I/O Registers
// - Owner     : Game Boy hardware (CPU, PPU, APU, Timer, Joypad, etc.)
// - Description: Controls system functions like input, timers, audio, display, serial, interrupts.
//                Example: 0xFF00 = joypad, 0xFF40 = LCDC, 0xFF04 = divider, 0xFF0F = IF (interrupt flags)

// 0xFF80 - 0xFFFE : HRAM (High RAM / Zero Page)
// - Owner     : Game Boy CPU
// - Description: Fast internal RAM.
//                Used for temporary storage and interrupt handling. 127 bytes total.

// 0xFFFF : Interrupt Enable Register
// - Owner     : Game Boy CPU
// - Description: Each bit enables a specific interrupt (VBlank, LCD, Timer, Serial, Joypad).
//                Must be implemented to support interrupts correctly.


//We will read an adress from the cartridge and depending on that mem address we will execute a function
//the cond checks anything before the end so add < 0x8000 says anything before 0x8000
u8 bus_read(u16 address){
    if(address < 0x8000){//for ROM banks
        return cart_read(address);
    }
    else if(address < 0xA000){ //from chr ram to end of bg map 2
        printf("Trying to read from bg tiles @ 0x%04X\n", address);
        NO_IMPL
    }
    else if(address < 0xC000){//cartridge ram where saves go to. Here we read the saves
        return cart_read(address);
    }
    else if(address < 0xE000){ //ram bank 0 to end of ram bank 1-7
        //WRAM working ram
        return wram_read(address);
    }
    else if(address < 0xFE00){ //reserved or Echo RAM
        //not needed
        return 0;
    }
    else if(address < 0xFEA0 ){ //obj attribute
        printf("Trying to read from obj attribute @ 0x%04X\n", address);
        NO_IMPL
    }
    else if(address < 0xFF00){ //reserved and unusable
        return 0;
    }
    else if(address < 0xFF80){ //IO registers
        printf("Trying to read from IO regs @ 0x%04X\n", address);
        NO_IMPL
    }
    else if(address == 0xFFFF){ //CPU enable register flag
        printf("Trying to read from CPU enable @ 0x%04X\n", address);
        NO_IMPL
    } 

    return hram_read(address); //the fast internal ram[end of the above table]
}

void bus_write(u16 address, u8 value){
    if(address < 0x8000){//for ROM banks
        cart_write(address, value);
    }
    else if(address < 0xA000){ //from chr ram to end of bg map 2
        printf("Trying to write to bg tiles @ 0x%04X\n", address);
        NO_IMPL
    }
    else if(address < 0xC000){//cartridge ram where saves go to. Here we read the saves
        cart_write(address, value);
    }
    else if(address < 0xE000){ //ram bank 0 to end of ram bank 1-7
        //WRAM working ram
        wram_write(address, value);
    }
    else if(address < 0xFE00){ //reserved or Echo RAM
        //not needed
    }
    else if(address < 0xFEA0 ){ //obj attribute
        printf("Trying to write to obj attribute @ 0x%04X\n", address);
        NO_IMPL
    }
    else if(address < 0xFF00){ //reserved and unusable
        //not needed
    }
    else if(address < 0xFF80){ //IO registers
        printf("Trying to write to IO regs @ 0x%04X\n", address);
        NO_IMPL
    }
    else if(address == 0xFFFF){ //CPU enable register flag
        printf("Trying to write to CPU enable @ 0x%04X\n", address);
        NO_IMPL
    }   
    else{
        hram_write(address, value);
    }

}