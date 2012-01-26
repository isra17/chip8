#include "Chip8_Renderer.h"

Uint8 Chip8_GetSpritePixel(Uint8* sprite, Uint8 x, Uint8 y) {
	return (sprite[y] & (0x80 >> x)) >> (7-x);
}

void Chip8_SetRendererPixel(Chip8_Renderer* renderer, Uint8 x, Uint8 y, Uint8 value) {
	renderer->screen_buffer[y*renderer->w + x] = value;
}

Uint8 Chip8_GetRendererPixel(Chip8_Renderer* renderer, Uint8 x, Uint8 y) {
	return renderer->screen_buffer[y*renderer->w + x];
}

Chip8_Renderer* Chip8_CreateRenderer(Uint8 w, Uint8 h) {
	Chip8_Renderer* renderer = malloc(sizeof(Chip8_Renderer));
	renderer->w = w;
	renderer->h = h;
	renderer->screen_buffer = malloc(w*h);
	memset(renderer->screen_buffer, 0, w*h);

	renderer->screen = SDL_SetVideoMode(w*SCREEN_RATIO, h*SCREEN_RATIO, 16, SDL_SWSURFACE);
	renderer->black = SDL_MapRGB(renderer->screen->format, 0, 0, 0);
	renderer->white = SDL_MapRGB(renderer->screen->format, 255, 255, 255);

	renderer->pixel[0] = SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_RATIO, SCREEN_RATIO, 16, 0, 0, 0, 0);
	SDL_FillRect(renderer->pixel[0], NULL, renderer->black);
	renderer->pixel[1] = SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_RATIO, SCREEN_RATIO, 16, 0, 0, 0, 0);
	SDL_FillRect(renderer->pixel[1], NULL, renderer->white);
	return renderer;
}

void Chip8_FreeRenderer(Chip8_Renderer* renderer) {
	SDL_FreeSurface(renderer->pixel[0]);
	SDL_FreeSurface(renderer->pixel[1]);
	SDL_FreeSurface(renderer->screen);	
	free(renderer->screen_buffer);
	free(renderer);
}

void Chip8_ClearRenderer(Chip8_Renderer* renderer){
	memset(renderer->screen_buffer, 0, renderer->w * renderer->h);
}

Uint8 Chip8_DrawSprite(Chip8_Renderer* renderer, Uint8* sprite, Uint8 bytes, Uint8 x, Uint8 y){
	int i,j;
	Uint8 col=0;
	Uint8 px, py, pixel;
	for(i=0; i < 8; i++){
		for(j=0; j < bytes; j++){
			px = i+x;
			if(i+x < 0)
				px = renderer->w + (i+x);
			else if(i+x >= renderer->w)
				px = i+x - renderer->w;

			py = j+y;
			if(j+y < 0)
				py = renderer->h + (j+y);
			else if(j+y >= renderer->h)
				py = j+y - renderer->h;

			pixel = Chip8_GetRendererPixel(renderer, px, py) ^ Chip8_GetSpritePixel(sprite, i, j);
			if(pixel == 0 && Chip8_GetRendererPixel(renderer, px, py) == 1)
				col = 1;
			Chip8_SetRendererPixel(renderer, px, py, pixel);
		}
	}

	return col;
}

void Chip8_Render(Chip8_Renderer* renderer){
	int i,j;
	SDL_Rect dest;
	dest.h = SCREEN_RATIO;
	dest.w = SCREEN_RATIO;

	SDL_FillRect(renderer->screen, NULL, renderer->black);

	for(i=0; i < renderer->w; i++){
		for(j=0; j < renderer->h; j++){
			dest.x = i * SCREEN_RATIO;
			dest.y = j * SCREEN_RATIO;
			SDL_BlitSurface(renderer->pixel[Chip8_GetRendererPixel(renderer, i, j)], NULL, renderer->screen, &dest);
		}
	}

	SDL_Flip(renderer->screen);
}
