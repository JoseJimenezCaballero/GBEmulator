CC = gcc
CFLAGS = -Wall -Wextra -I./include -I/opt/homebrew/Cellar/sdl2/2.32.2/include/SDL2 -I/opt/homebrew/Cellar/sdl2_ttf/2.24.0/include/SDL2
LDFLAGS = -L/opt/homebrew/Cellar/sdl2/2.32.2/lib -L/opt/homebrew/Cellar/sdl2_ttf/2.24.0/lib -lSDL2 -lSDL2_ttf

SRCS = gbemu/main.c lib/bus.c lib/cart.c lib/cpu.c lib/emu.c
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

