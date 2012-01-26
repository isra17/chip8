#ifndef CHIP8_INSTRUCTION_H
#define CHIP8_INSTRUCTION_H

#include <SDL.h>

struct Chip8_Machine;
struct Chip8_Instruction;

typedef struct Chip8_Instruction
{
	Uint16 mask;
	Uint16 value;
	void(*handler)(struct Chip8_Machine *, Uint16);
} Chip8_Instruction;

Chip8_Instruction* Chip8_GetInstruction(struct Chip8_Machine* machine, Uint16* opcode);

#endif