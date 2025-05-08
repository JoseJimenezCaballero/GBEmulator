#pragma once
/*Just macros and what not here*/


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t u8; //shortcuts for bits so we dont type uint8_t all the time
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)

#define BIT_SET(a, n, on) (on ? (a) |= (1 << n) : (a) &= ~(1 << n))

#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

void delay(u32 ms);

#define NO_IMPL { fprintf(stderr, "NOT YET IMPLEMENTED\n");exit(-1);} //macro for stuff that hasnt been implemented yet

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10

typedef int8_t s8; //signed integer