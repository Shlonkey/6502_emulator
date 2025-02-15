#pragma once

#include<stdint.h>

#define PAGE_COUNT (0xFF + 1)
#define PAGE_SIZE (0xFF + 1)

typedef uint8_t byte;

struct CPU
{
	byte memory[PAGE_COUNT][PAGE_SIZE];

	byte PCL;
	byte PCH;

	byte SP;
	byte A;
	byte X;
	byte Y;

	byte N : 1;
	byte V : 1;
	byte B : 1;
	/*UNSUSED*/
	byte D : 1;
	byte I : 1;
	byte Z : 1;
	byte C : 1;
};

void cpu_reset(struct CPU* p_cpu);
byte fetch_byte(struct CPU* p_cpu);
byte read_byte(struct CPU* p_cpu);

void cpu_pc_increment(struct CPU* p_cpu);
