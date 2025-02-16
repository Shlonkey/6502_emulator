#pragma once

#include<stdint.h>
#include<time.h>

#define PAGE_COUNT (0xFF + 1)
#define PAGE_SIZE (0xFF + 1)

#define CLOCK_FREQUENCY_HZ (1790000)
#define CLOCK_PERIOD_NSEC (1000000000 / (CLOCK_FREQUENCY_HZ))

#define NMI_LO 0xFA
#define NMI_HI 0xFF
#define RESET_LO 0xFC
#define RESET_HI 0xFF
#define IRQ_LO 0xFE
#define IRQ_HI 0xFF

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
	byte D : 1;
	byte I : 1;
	byte Z : 1;
	byte C : 1;
};

void cpu_wait_clock_cycle(void);
void cpu_power_on(struct CPU* p_cpu);
void cpu_reset(struct CPU* p_cpu);
void cpu_memory_randomize(struct CPU* p_cpu);

void cpu_write_byte(struct CPU* p_cpu, byte value, byte address_lo, byte address_hi);
byte cpu_fetch_byte(struct CPU* p_cpu);
byte cpu_read_byte(struct CPU* p_cpu);

void cpu_pc_increment(struct CPU* p_cpu);
