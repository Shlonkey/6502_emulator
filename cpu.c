#include<stdlib.h>

#include"cpu.h"

void cpu_power_on(struct CPU* p_cpu)
{
	p_cpu->A = 0x00;
	p_cpu->X = 0x00;
	p_cpu->Y = 0x00;

	p_cpu->SP = 0xFD;

	p_cpu->N = 0;
	p_cpu->V = 0;
	p_cpu->D = 0;
	p_cpu->I = 1;
	p_cpu->Z = 0;
	p_cpu->C = 0;

	cpu_memory_randomize(p_cpu);
}

void cpu_reset(struct CPU* p_cpu)
{
	p_cpu->PCL = RESET_LO;
	p_cpu->PCH = RESET_HI;

	byte effective_address_lo = cpu_fetch_byte(p_cpu);
	byte effective_address_hi = cpu_fetch_byte(p_cpu);

	p_cpu->PCL = effective_address_lo;
	p_cpu->PCH = effective_address_hi;

	return;
}

void cpu_memory_randomize(struct CPU* p_cpu)
{
	for(short page_number = 0; page_number < PAGE_COUNT; page_number++)
	{
		for(short page_index = 0; page_index < PAGE_SIZE; page_index++)
		{
			p_cpu->memory[page_number][page_index] = (byte)rand();
		}
	}
}

byte cpu_fetch_byte(struct CPU* p_cpu)
{
	byte value = cpu_read_byte(p_cpu);
	cpu_pc_increment(p_cpu);
	return value;
}

byte cpu_read_byte(struct CPU* p_cpu)
{
	cpu_wait_clock_cycle();
	return p_cpu->memory[p_cpu->PCH][p_cpu->PCL];
}

void cpu_write_byte(struct CPU* p_cpu, byte value, byte address_lo, byte address_hi)
{
	p_cpu->memory[address_hi][address_lo] = value; 
	cpu_wait_clock_cycle();
}

void cpu_pc_increment(struct CPU* p_cpu)
{
	uint16_t pc = ((p_cpu->PCH) << 8) + (p_cpu->PCL);
	pc++;
	p_cpu->PCH = (byte)((pc & (0xFF00)) >> 8);
	p_cpu->PCL = (byte)(pc & 0x00FF);
	return;
}

void cpu_wait_clock_cycle()
{
	struct timespec ts = {.tv_sec = 0, .tv_nsec = CLOCK_PERIOD_NSEC};
	struct timespec remaining;

	while( nanosleep(&ts, &remaining) != 0)
	{
		ts.tv_nsec = remaining.tv_nsec;
		ts.tv_sec = remaining.tv_sec;
	}

	return;
}	
