#ifndef CHIP8_INPUT_H
#define CHIP8_INPUT_H

#include <SDL.h>

Uint8 Chip8_PollKey(Uint8 key);
Uint8 Chip8_WaitKey();

#endif

