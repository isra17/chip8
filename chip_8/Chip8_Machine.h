#ifndef CHIP8_MACHINE_H
#define CHIP8_MACHINE_H

#include <SDL.h>

struct Chip8_Renderer;

typedef union Chip8_Mem {
	struct {
		Uint8 padding[512];
		Uint8 program[3584];
	} mem;
	Uint8 mem_pool[4096];
} Chip8_Mem;

typedef union Chip8_Registry {
	struct {
		Uint8 V0;
		Uint8 V1;
		Uint8 V2;
		Uint8 V3;
		Uint8 V4;
		Uint8 V5;
		Uint8 V6;
		Uint8 V7;
		Uint8 V8;
		Uint8 V9;
		Uint8 VA;
		Uint8 VB;
		Uint8 VC;
		Uint8 VD;
		Uint8 VE;
		Uint8 VF;
	}named_reg;
	Uint8 reg[16];
} Chip8_Registry;

typedef struct Chip8_Machine {
	Chip8_Mem mem;
	Chip8_Registry registry;
	Uint16 stack[16];
	Uint16 pc;
	Uint8 sp;
	Uint8 dt;
	Uint8 st;
	Uint16 I;
	Uint32 clockrate;
	Uint8 is_running;
	struct Chip8_Renderer* renderer;
} Chip8_Machine;

Chip8_Machine*	Chip8_CreateMachine(char*);
int				Chip8_ExecuteMachine(Chip8_Machine* machine);
int				Chip8_RunMachine(Chip8_Machine* machine);
void			Chip8_Pause(Chip8_Machine* machine);
void			Chip8_Resume(Chip8_Machine* machine);
void			Chip8_LoadRom(Chip8_Machine* machine, char* path);

#endif