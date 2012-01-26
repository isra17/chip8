#include <SDL.h>
#include <time.h>
#include <stdlib.h>

#include "Chip8_Machine.h"
#include "Chip8_Instruction.h"

//Load sample program
void LoadProgram(Chip8_Machine* machine){
	Uint8* program = machine->mem.mem.program;
	Uint8 my_program[] = {
		0xA2, 0x08, 0xDA, 0xA2, 0x8A, 0xB1, 0x12, 0x04, 0xAA, 0x00
	};

	memcpy(program, my_program, sizeof(my_program));
}

int main(int argc, char* args[]){
	char* rom_path = "BREAKOUT";
	Chip8_Machine* machine = 0;
	if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
	atexit(SDL_Quit);

	if(argc == 2){
		rom_path = args[1];
	}

	srand ( (unsigned int)time(NULL) );

	machine = Chip8_CreateMachine(rom_path);
	return Chip8_RunMachine(machine);;
}