#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>

#define MEMORY_SIZE (0xFFFF + 1)
#define CLOCK_FREQUENCY 1000.0
#define CLOCK_TIME (1 / CLOCK_FREQUENCY)

#define CLOCK_CYCLE() (sleep(CLOCK_TIME))

#define INS_BRK_IMMEDIATE 0x00
#define INS_JSR_ABSOLUTE 0x20

#define INS_LDA_IMMEDIATE 0xA9
#define INS_LDA_ZEROPAGE  0xA5
#define INS_LDA_ZEROPAGE_X 0xB5


typedef uint8_t byte;
typedef uint16_t word;

struct CPU
{
	//Program Counter
	byte PCL;
	byte PCH;

	//Stack Pointer
	byte SP;
	
	//Accumulator, X & Y
	byte A, X, Y;

	//Processor Status
	byte N : 1; //Negative
	byte V : 1; //Overflow
	byte B : 1; //Break INfo
	byte D : 1; //Decimal
	byte I : 1; //Interupt Disable
	byte Z : 1; //Zero
	byte C : 1; //Carry

	byte memory[MEMORY_SIZE];
};

void set_pc(struct CPU* p_cpu, word address)
{
	p_cpu->PCH = (address & 0xFF00) >> 8;
	p_cpu->PCL = (address & 0x00FF);
	return;
}

word form_word(byte HI, byte LO)
{
	return (HI << 8) | LO;
}

word get_pc(struct CPU* p_cpu)
{
	return form_word(p_cpu->PCH, p_cpu->PCL);
}

void increment_pc(struct CPU* p_cpu)
{
	word PC = get_pc(p_cpu);
	set_pc(p_cpu, PC + 1);
	return;
}

byte read_byte(struct CPU* p_cpu, word address)
{
	CLOCK_CYCLE();
	return p_cpu->memory[address];
}

byte fetch_byte(struct CPU* p_cpu)//1 cycle
{
	word address = get_pc(p_cpu);
	byte opcode = read_byte(p_cpu, address);
	increment_pc(p_cpu);
	return opcode;
}

void update_NZ_flags(struct CPU* p_cpu, byte value)
{
	p_cpu->Z = (value == 0);
	p_cpu->N = ((value & 0b10000000) >> 7 == 1);
	return;
}

void push_to_stack(struct CPU* p_cpu, byte value)
{
	p_cpu->memory[0x0100 + p_cpu->SP] = value;
	p_cpu->SP -= 1;	
	CLOCK_CYCLE();
	return;
}

void reset_cpu(struct CPU* p_cpu)	//Power on behaviour
{
	set_pc(p_cpu, 0xFFFC);	//Go to reset vector location.
	byte LO = fetch_byte(p_cpu);
	byte HI = fetch_byte(p_cpu);
	word reset_address = form_word(HI, LO);
	set_pc(p_cpu, reset_address);

	p_cpu->SP = 0xFD;	//Sets SP to default offset.

	return;
}

void DEBUG_CPU_INFO(struct CPU* p_cpu)
{
	byte A = p_cpu->A;
	byte X = p_cpu->X;
	byte Y = p_cpu->Y;
	byte SP = p_cpu->SP;
	word PC = get_pc(p_cpu);
	printf("A:0x%02X, X:0x%02X, Y:0x%02X, SP:0x%02X, PC:0x%04X\nN:%b, V:%b, Z:%b\n\n",A, X, Y, SP, PC, p_cpu->N, p_cpu->V, p_cpu->Z);	
}

int main(int argc, char* argv[])
{
	struct CPU cpu;
	
	//TEMPORARY ROM
	cpu.memory[0xFFFC] = 0x00;//These lines dictate where program execution will start.
	cpu.memory[0xFFFD] = 0x06;//
	
	cpu.memory[0x0600] = INS_LDA_IMMEDIATE;
	cpu.memory[0x0601] = 0x42;
	cpu.memory[0x0602] = INS_JSR_ABSOLUTE;
	cpu.memory[0x0603] = 0x00;
	cpu.memory[0x0604] = 0x08;
	cpu.memory[0x0605] = 0x00;

	cpu.memory[0x0800] = INS_LDA_IMMEDIATE;
	cpu.memory[0x0801] = 0x69;
	cpu.memory[0x0802] = 0x00;
	
	reset_cpu(&cpu);
	
	byte value;
	byte zp_address;
	byte unused_byte;	
	byte LO;
	byte HI;
		//Execute.
	while(1)
	{
		DEBUG_CPU_INFO(&cpu);
		byte opcode = fetch_byte(&cpu);
		switch (opcode)
		{
			case INS_LDA_IMMEDIATE:
				value = fetch_byte(&cpu);
				cpu.A = value;	//Immediatly Loads Accumulator		
				update_NZ_flags(&cpu, cpu.A);	//Determines new flags
				break;

			case INS_LDA_ZEROPAGE:
				zp_address = fetch_byte(&cpu);
				cpu.A = read_byte(&cpu, zp_address);	//Loads value in zero page at location 'operand'
				update_NZ_flags(&cpu, cpu.A);
				break;
			
			case INS_LDA_ZEROPAGE_X:
				byte zp_address = fetch_byte(&cpu);
				zp_address += cpu.X;
				CLOCK_CYCLE();	//Used to add time to adding X to the address.
				cpu.A = read_byte(&cpu, zp_address);
				update_NZ_flags(&cpu, cpu.A);
				break;

			case INS_JSR_ABSOLUTE:
				//LO = fetch_byte(&cpu);
				//HI = fetch_byte(&cpu);	
				//word return_address = get_pc(&cpu) - 1;
				//CLOCK_CYCLE();//Some wierd stuff here
				//push_to_stack(&cpu, (return_address & 0xFF00) >> 8);
				//push_to_stack(&cpu, return_address & 0x00FF);
				//cpu.PCL = LO;
				//cpu.PCH = HI;
				//break;
				
				LO = fetch_byte(&cpu);
				CLOCK_CYCLE();
				
				push_to_stack(&cpu, 0x00);
				push_to_stack(&cpu, 0x08);
				
				cpu.PCH = fetch_byte(&cpu);
				cpu.PCL = LO;
				break;

			case INS_BRK_IMMEDIATE:
				unused_byte = fetch_byte(&cpu);
				goto program_termination;

			default:
				printf("Instruction '0x%02X' Not Handled\n");
				break;
		}
	}
	program_termination:
	DEBUG_CPU_INFO(&cpu);
	return 0;
}
