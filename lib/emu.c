#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include "/opt/homebrew/Cellar/sdl2/2.32.2/include/SDL2/SDL.h"
#include "/opt/homebrew/Cellar/sdl2_ttf/2.24.0/include/SDL2/SDL_ttf.h"

static emu_context ctx; //create an emu context

emu_context *emu_get_context(){//return the context
    return &ctx;
}

void delay(u32 ms){//used for delaying
    SDL_Delay(ms);
}

int emu_run(int argc, char **argv){
    if(argc < 2){ //error checking
        printf("Error load a file\n");
        return -1;
    }

    if(!cart_load(argv[1])){
        printf("Failed to load ROM file: %s\n", argv[1]);
        return -2;
    }

    printf("Cart Loaded Successfully\n");

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();//used for fonts

    cpu_init();//start cpu 

    ctx.running = true;//init the values for context
    ctx.paused = false;
    ctx.ticks = 0;

    while(ctx.running){//infinite loop
        if(ctx.paused){
            delay(10);
            continue;
        }

        if(!cpu_step()){
            printf("CPU Stopped\n");
            return -3;
        }

        ctx.ticks++;//increase ticks per successfull cpu run
    }

    return 0;
}