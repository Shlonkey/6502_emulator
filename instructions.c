#include"instructions.h"

void (*instructions_decode_opcode(byte opcode))(struct CPU*, byte)
{
	switch(opcode){
	case LDA_IMMEDIATE:
		return lda_immediate;
	default:
		return nop;
	}
}

void nop(struct CPU* p_cpu, byte operand)
{
	return;
}

void lda_immediate(struct CPU* p_cpu, byte operand)
{
	return;
}
