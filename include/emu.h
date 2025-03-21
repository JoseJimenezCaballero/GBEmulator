#pragma once

#include "common.h"

typedef struct { //data about the running emulator 
    bool paused;
    bool running;
    u64 ticks; //timer ticks
} emu_context;

int emu_run(int argc, char **argv);

emu_context *emu_get_context();