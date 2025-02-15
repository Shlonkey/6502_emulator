#include"cpu.h"

void cpu_reset(struct CPU* p_cpu)
{
	p_cpu->PCL = 0xFC;
	p_cpu->PCH = 0xFF;

	byte LO = fetch_byte(p_cpu);
	byte HI = fetch_byte(p_cpu);

	p_cpu->LO = LO;
	p_cpu->HI = HI;

	p_cpu->SP = 0xFD;	
}

byte fetch_byte(struct CPU* p_cpu)
{
	byte value = read_byte(p_cpu);
	cpu_pc_increment(p_cpu)
}

byte read_byte(struct CPU* p_cpu)
{
	wait_clock_cycle();
	return p_cpu->memory[p_cpu->PCL][p_cpu->PCH];
}

void cpu_pc_increment(struct CPU* p_cpu)
{
	uint16_t pc = ((p_cpu->PCH) << 8) + (p_cpu->PCL);
	pc++;
	p_cpu->PCH = (byte)((pc & (0xFF00)) >> 8);
	p_cpu->PCL = (byte)(pc & 0x00FF);
	return;
}

/*Implement to wait for time representing clock period*/
void wait_clock_cycle()
{
	return;
}	
