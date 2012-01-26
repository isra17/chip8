#include <stdio.h>

#include "Chip8_Machine.h"
#include "Chip8_Renderer.h"
#include "Chip8_Instruction.h"

Uint8 charset[16][5] = {
	{0xF0,0x90,0x90,0x90,0xF0},
	{0x20,0x60,0x20,0x20,0x70},
	{0xF0,0x10,0xF0,0x80,0xF0},
	{0xF0,0x10,0xF0,0x10,0xF0},
	{0x90,0x90,0xF0,0x10,0x10},
	{0xF0,0x80,0xF0,0x10,0xF0},
	{0xF0,0x80,0xF0,0x90,0xF0},
	{0xF0,0x10,0x20,0x40,0x40},
	{0xF0,0x90,0xF0,0x90,0xF0},
	{0xF0,0x90,0xF0,0x10,0xF0},
	{0xF0,0x90,0xF0,0x90,0x90},
	{0xE0,0x90,0xE0,0x90,0xE0},
	{0xF0,0x80,0x80,0x80,0xF0},
	{0xE0,0x90,0x90,0x90,0xE0},
	{0xF0,0x80,0xF0,0x80,0xF0},
	{0xF0,0x80,0xF0,0x80,0x80}
};

void Chip8_LoadRom(Chip8_Machine* machine, char* path){
	FILE* program_file = fopen(path, "rb");
	fread(machine->mem.mem.program, sizeof(machine->mem.mem.program), 1, program_file);	
}

Chip8_Machine* Chip8_CreateMachine(char* rom_path) {
	Chip8_Machine* machine = malloc(sizeof(Chip8_Machine));
	memset(&machine->mem, 0, sizeof(Chip8_Mem));
	memset(&machine->registry, 0, sizeof(Chip8_Registry));
	memset(machine->stack, 0, sizeof(machine->stack));
	memcpy(machine->mem.mem_pool, charset, sizeof(charset));

	Chip8_LoadRom(machine, rom_path);

	machine->pc = sizeof(machine->mem.mem.padding);
	machine->sp = 0;
	machine->dt = 0;
	machine->st = 0;
	machine->I = 0;
	machine->clockrate = 500;
	machine->renderer = Chip8_CreateRenderer(64,32);
	return machine;
}

void Chip8_FreeMachine(Chip8_Machine* machine) {
	Chip8_FreeRenderer(machine->renderer);
	free(machine);
}

int Chip8_ExecuteMachine(Chip8_Machine* machine) {
	Uint16 instr_pointer;
	Chip8_Instruction* instruction = Chip8_GetInstruction(machine, &instr_pointer);
	(*instruction->handler)(machine, instr_pointer);
	return 0;
}

int Chip8_RunMachine(Chip8_Machine* machine) {	
	Uint32	timer_tick_remain = 0,
			timer_elapsed_tick = 0,
			timer_clock_tick = 1000 / 60,
			timer_last_tick = SDL_GetTicks();

	Uint32	sys_tick_remain = 0,
			sys_elapsed_tick = 0,
			sys_clock_tick = 1000 / machine->clockrate,
			sys_last_tick = SDL_GetTicks();

	Uint32	current_tick;
	int a=0,b=0;

	while(machine->is_running){
		current_tick = SDL_GetTicks();
		if(current_tick - sys_last_tick >= sys_clock_tick){
			Chip8_ExecuteMachine(machine);
			sys_last_tick = current_tick;
		}
		sys_elapsed_tick = (current_tick - sys_last_tick);
		sys_tick_remain = sys_clock_tick > sys_elapsed_tick? sys_clock_tick - sys_elapsed_tick: 0;

		if(current_tick - timer_last_tick >= timer_clock_tick){
			//decrement timer registry
			if(machine->dt > 0)
				machine->dt--;
			if(machine->st > 0)
				machine->st--;

			Chip8_Render(machine->renderer);

			timer_last_tick = current_tick;
		}
		timer_elapsed_tick = (current_tick - timer_last_tick);
		timer_tick_remain = timer_clock_tick > timer_elapsed_tick? timer_clock_tick - timer_elapsed_tick: 0;

		SDL_Delay(sys_tick_remain < timer_tick_remain? sys_tick_remain: timer_tick_remain);
	}

	return 0;
}

