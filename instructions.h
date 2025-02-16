#pragma once

#include<stdint.h>

#include"cpu.h"

#define JAM 0x02 //Illegal opcode used for now to terminate program.
#define LDA_IMMEDIATE 0xA9

void (*instructions_decode_opcode(byte opcode))(struct CPU*, byte);

void lda_immediate(struct CPU* p_cpu, byte operand);
void nop(struct CPU* p_cpu, byte operand);
