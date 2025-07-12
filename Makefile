CC = gcc
CFLAGS = `sdl2-config --cflags` -Iinclude
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf


SRCS = gbemu/main.c lib/bus.c lib/cart.c lib/cpu.c lib/emu.c lib/instructions.c lib/stack.c lib/ram.c
OBJS = $(SRCS:%.c=%.o)

EXEC = main

# Targets
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

