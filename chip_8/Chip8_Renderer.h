#ifndef CHIP8_RENDERER_H
#define CHIP8_RENDERER_H

#include <SDL.h>

#define SCREEN_RATIO 10

typedef struct Chip8_Renderer {
	SDL_Surface *screen;
	SDL_Surface* pixel[2];
	Uint8 *screen_buffer;
	Uint8 w;
	Uint8 h;
	Uint32 black;
	Uint32 white;
} Chip8_Renderer;

Uint8			Chip8_GetSpritePixel(Uint8* sprite, Uint8 x, Uint8 y);
Uint8			Chip8_GetRendererPixel(Chip8_Renderer* renderer, Uint8 x, Uint8 y);
void			Chip8_SetRendererPixel(Chip8_Renderer* renderer, Uint8 x, Uint8 y, Uint8 value);

Chip8_Renderer*	Chip8_CreateRenderer(Uint8 w, Uint8 h);
void			Chip8_FreeRenderer(Chip8_Renderer* renderer);
void			Chip8_ClearRenderer(Chip8_Renderer* renderer);
Uint8			Chip8_DrawSprite(Chip8_Renderer* renderer, Uint8* sprite, Uint8 bytes, Uint8 x, Uint8 y);
void			Chip8_Render(Chip8_Renderer* renderer);

#endif