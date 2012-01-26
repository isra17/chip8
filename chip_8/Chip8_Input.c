#include "Chip8_Input.h"

typedef struct KeyMap{
	Uint8 chip8_key;
	SDLKey sdl_key;
} KeyMap;

KeyMap KEY_MAP[] = {
	{0x0, SDLK_x},
	{0x1, SDLK_1},
	{0x2, SDLK_2},
	{0x3, SDLK_3},
	{0x4, SDLK_q},
	{0x5, SDLK_w},
	{0x6, SDLK_e},
	{0x7, SDLK_a},
	{0x8, SDLK_s},
	{0x9, SDLK_d},
	{0xA, SDLK_z},
	{0xB, SDLK_c},
	{0xC, SDLK_4},
	{0xD, SDLK_r},
	{0xE, SDLK_f},
	{0xF, SDLK_v},
};

Uint8 Chip8_PollKey(Uint8 key){
	int map = -1, i;

	SDL_PumpEvents();

	for(i=0; i<sizeof(KEY_MAP) && map == -1; i++){
		if(KEY_MAP[i].chip8_key == key){
			map = i;
		}
	}
	
	if(map != -1){
		Uint8* key_state = SDL_GetKeyState(NULL);
		return key_state[KEY_MAP[map].sdl_key];
	}
	return 0;
}

Uint8 Chip8_WaitKey(){
	SDL_Event event;
	Uint8 key, key_pressed=0;

	while(!key_pressed){
		SDL_WaitEvent(&event);
		if(event.type == SDL_KEYDOWN){
			int i;
			for(i=0; i<sizeof(KEY_MAP) && !key_pressed; i++){
				if(KEY_MAP[i].sdl_key == event.key.keysym.sym){
					key = KEY_MAP[i].chip8_key;
					key_pressed = 1;
				}
			}
		}
	}

	return key;
}