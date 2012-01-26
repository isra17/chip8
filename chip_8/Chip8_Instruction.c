#include "Chip8_Instruction.h"
#include "Chip8_Machine.h"
#include "Chip8_Renderer.h"
#include "Chip8_Input.h"

#include <stdio.h>

void Chip8_PrintInstruction(Chip8_Machine* machine, Uint16 opcode) {
	printf("%04X %04X\n",machine->pc, opcode);
}

void Chip8_StopInstruction(Chip8_Machine* machine, Uint16 opcode){
	machine->is_running = 0;
}

void Chip8_0NNN_SYS(Chip8_Machine* machine, Uint16 opcode){
}

void Chip8_00E0_CLS(Chip8_Machine* machine, Uint16 opcode){
	Chip8_ClearRenderer(machine->renderer);
}

void Chip8_00EE_RET(Chip8_Machine* machine, Uint16 opcode){
	if(machine->sp == 0)
		machine->is_running = 0;
	else{
		machine->pc = machine->stack[machine->sp - 1];
		machine->sp--;
	}
}

void Chip8_1NNN_JP(Chip8_Machine* machine, Uint16 opcode){
	Uint16 addr = opcode & 0x0FFF;
	machine->pc = addr;
}

void Chip8_2NNN_CALL(Chip8_Machine* machine, Uint16 opcode){
	Uint16 addr = opcode & 0x0FFF;
	machine->stack[machine->sp] = machine->pc;
	machine->sp++;
	machine->pc = addr;
}

void Chip8_3XKK_SE(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 value = opcode & 0x00FF;

	if(machine->registry.reg[x] == value)
		machine->pc += 2;
}

void Chip8_4XKK_SNE(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 value = opcode & 0x00FF;

	if(machine->registry.reg[x] != value)
		machine->pc += 2;
}

void Chip8_5XY0_SE(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 y = (opcode & 0x00F0) >> 4;

	if(machine->registry.reg[x] == machine->registry.reg[y])
		machine->pc += 2;
}

void Chip8_6XKK_LD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 value = opcode & 0x00FF;

	machine->registry.reg[x] = value;
}

void Chip8_7XKK_ADD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 value = opcode & 0x00FF;

	machine->registry.reg[x] += value;
}

void Chip8_8XY0_LD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 y = (opcode & 0x00F0) >> 4;

	machine->registry.reg[x] = machine->registry.reg[y];
}

void Chip8_8XY1_OR(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 y = (opcode & 0x00F0) >> 4;

	machine->registry.reg[x] |= machine->registry.reg[y];
}

void Chip8_8XY2_AND(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 y = (opcode & 0x00F0) >> 4;

	machine->registry.reg[x] &= machine->registry.reg[y];
}

void Chip8_8XY3_XOR(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 y = (opcode & 0x00F0) >> 4;

	machine->registry.reg[x] ^= machine->registry.reg[y];
}

void Chip8_8XY4_ADD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 y = (opcode & 0x00F0) >> 4;

	Uint8 Vx = machine->registry.reg[x];
	Uint8 Vy = machine->registry.reg[y];

	machine->registry.named_reg.VF = (0xFF - Vx) < Vy;
	machine->registry.reg[x] += Vy;
}

void Chip8_8XY5_SUB(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 y = (opcode & 0x00F0) >> 4;
	
	Uint8 Vx = machine->registry.reg[x];
	Uint8 Vy = machine->registry.reg[y];
	
	machine->registry.named_reg.VF = Vx >= Vy;
	machine->registry.reg[x] -= Vy;
}

void Chip8_8XY6_SHR(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;

	Uint8 Vx = machine->registry.reg[x];

	machine->registry.named_reg.VF = Vx & 1;
	machine->registry.reg[x] /= 2;
}

void Chip8_8XY7_SUBN(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 y = (opcode & 0x00F0) >> 4;
	
	Uint8 Vx = machine->registry.reg[x];
	Uint8 Vy = machine->registry.reg[y];
	
	machine->registry.named_reg.VF = Vy >= Vx;
	machine->registry.reg[x] = Vy - Vx;
}

void Chip8_8XYE_SHL(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;

	Uint8 Vx = machine->registry.reg[x];

	machine->registry.named_reg.VF = (Vx & 0x80) != 0;
	machine->registry.reg[x] <<= 1;
}

void Chip8_9XY0_SNE(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 y = (opcode & 0x00F0) >> 4;

	if(machine->registry.reg[x] != machine->registry.reg[y])
		machine->pc += 2;
}

void Chip8_ANNN_LD(Chip8_Machine* machine, Uint16 opcode){
	Uint16 value = (opcode & 0x0FFF);
	machine->I = value;
}

void Chip8_BNNN_JP(Chip8_Machine* machine, Uint16 opcode){
	Uint16 value = (opcode & 0x0FFF);
	machine->pc = value + machine->registry.named_reg.V0;
}

void Chip8_CXKK_RND(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint16 value = (opcode & 0x0FFF);

	machine->registry.reg[x] = (rand() % 256) & value;
}

void Chip8_DXYN_DRW(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 y = (opcode & 0x00F0) >> 4;
	Uint8 n = (opcode & 0x000F);
	Uint8* sprite = machine->mem.mem_pool + machine->I;
	Uint8 px = machine->registry.reg[x];
	Uint8 py = machine->registry.reg[y];

	Uint8 col = Chip8_DrawSprite(machine->renderer, sprite, n, px, py);
	machine->registry.named_reg.VF = col;
}

void Chip8_EX9E_SKP(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;

	Uint8 state = Chip8_PollKey(machine->registry.reg[x]);
	if(state)
		machine->pc += 2;
}

void Chip8_EXA1_SKNP(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;

	Uint8 state = Chip8_PollKey(machine->registry.reg[x]);
	if(!state)
		machine->pc += 2;
}

void Chip8_FX07_LD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;

	machine->registry.reg[x] = machine->dt;
}

void Chip8_FX0A_LD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;

	machine->registry.reg[x] = Chip8_WaitKey();
}

void Chip8_FX15_LD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;

	machine->dt = machine->registry.reg[x];
}

void Chip8_FX18_LD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;

	machine->st = machine->registry.reg[x];
}

void Chip8_FX1E_ADD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint8 Vx = machine->registry.reg[x];

	machine->registry.named_reg.VF = (0xFFF - machine->I) < Vx;
	machine->I += machine->registry.reg[x];
}

void Chip8_FX29_LD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;

	machine->I = machine->registry.reg[x] * 5;
}

void Chip8_FX33_LD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8;
	Uint16 I = machine->I;
	Uint8 Vx = machine->registry.reg[x];

	machine->mem.mem_pool[I] = Vx/100;
	machine->mem.mem_pool[I+1] = (Vx%100)/10;
	machine->mem.mem_pool[I+2] = Vx%10;
}

void Chip8_FX55_LD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8, i;
	for(i=0; i<=x; i++){
		machine->mem.mem_pool[machine->I + i] = machine->registry.reg[i];
	}
}

void Chip8_FX65_LD(Chip8_Machine* machine, Uint16 opcode){
	Uint8 x = (opcode & 0x0F00) >> 8, i;
	for(i=0; i<=x; i++){
		machine->registry.reg[i] = machine->mem.mem_pool[machine->I + i];
	}
}

Chip8_Instruction INSTRUCTION_SET[] = {
	{0xFFFF, 0x0000, &Chip8_PrintInstruction},
	{0xFFFF, 0x00E0, &Chip8_00E0_CLS},
	{0xFFFF, 0x00EE, &Chip8_00EE_RET},
	{0xF000, 0x1000, &Chip8_1NNN_JP},
	{0xF000, 0x2000, &Chip8_2NNN_CALL},
	{0xF000, 0x3000, &Chip8_3XKK_SE},
	{0xF000, 0x4000, &Chip8_4XKK_SNE},
	{0xF00F, 0x5000, &Chip8_5XY0_SE},
	{0xF000, 0x6000, &Chip8_6XKK_LD},
	{0xF000, 0x7000, &Chip8_7XKK_ADD},
	{0xF00F, 0x8000, &Chip8_8XY0_LD},
	{0xF00F, 0x8001, &Chip8_8XY1_OR},
	{0xF00F, 0x8002, &Chip8_8XY2_AND},
	{0xF00F, 0x8003, &Chip8_8XY3_XOR},
	{0xF00F, 0x8004, &Chip8_8XY4_ADD},
	{0xF00F, 0x8005, &Chip8_8XY5_SUB},
	{0xF00F, 0x8006, &Chip8_8XY6_SHR},
	{0xF00F, 0x8007, &Chip8_8XY7_SUBN},
	{0xF00F, 0x800E, &Chip8_8XYE_SHL},
	{0xF00F, 0x9000, &Chip8_9XY0_SNE},
	{0xF000, 0xA000, &Chip8_ANNN_LD},
	{0xF000, 0xB000, &Chip8_BNNN_JP},
	{0xF000, 0xC000, &Chip8_CXKK_RND},
	{0xF000, 0xD000, &Chip8_DXYN_DRW},
	{0xF0FF, 0xE09E, &Chip8_EX9E_SKP},
	{0xF0FF, 0xE0A1, &Chip8_EXA1_SKNP},
	{0xF0FF, 0xF007, &Chip8_FX07_LD},
	{0xF0FF, 0xF00A, &Chip8_FX0A_LD},
	{0xF0FF, 0xF015, &Chip8_FX15_LD},
	{0xF0FF, 0xF018, &Chip8_FX18_LD},
	{0xF0FF, 0xF01E, &Chip8_FX1E_ADD},
	{0xF0FF, 0xF029, &Chip8_FX29_LD},
	{0xF0FF, 0xF033, &Chip8_FX33_LD},
	{0xF0FF, 0xF055, &Chip8_FX55_LD},
	{0xF0FF, 0xF065, &Chip8_FX65_LD},


	{0x0000, 0x0000, Chip8_PrintInstruction}
};

Chip8_Instruction* Chip8_GetInstruction(Chip8_Machine* machine, Uint16* opcode){
	Chip8_Instruction* instruction = NULL;
	int i;
	Uint8* pc = machine->mem.mem_pool + machine->pc;
	*opcode = (Uint16)((pc[0] << 8) + pc[1]);
		
	for(i = 0; i < sizeof(INSTRUCTION_SET) && instruction == NULL; i++){
		Uint16 masked_value = INSTRUCTION_SET[i].mask & *opcode;
		instruction = masked_value == INSTRUCTION_SET[i].value? &INSTRUCTION_SET[i]: NULL;
	}

	machine->pc += 2;
	return instruction;
}