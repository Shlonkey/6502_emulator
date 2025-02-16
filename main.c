#include<stdio.h>

#include"cpu.h"
#include"instructions.h"

int main(int argc, char* argv[])
{
	struct CPU cpu;

	cpu_power_on(&cpu);
	
	//Temporary setting of RESET VECTOR
	cpu_write_byte(&cpu, 0x00, RESET_LO, RESET_HI);
	cpu_write_byte(&cpu, 0x02, RESET_LO + 1, RESET_HI);

	cpu_reset(&cpu);

	printf("PC : 0x%02X%02X\n", cpu.PCH, cpu.PCL);

	while(1)
	{
		//Fetch
		byte opcode = cpu_fetch_byte(&cpu);
		byte operand = cpu_fetch_byte(&cpu);
		
		//Decode
		void (*operation)(struct CPU*, byte) = instructions_decode_opcode(opcode);
		
		//Execute
		(*operation)(&cpu, operand);

		//TEMP
		if(opcode == JAM)
			break;
	}


	
	

	return 0;
}
