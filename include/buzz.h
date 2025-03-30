#ifndef BUZZ_H
#define BUZZ_H

#include "game.h"

#define BUZZ_WIDTH 90
#define BUZZ_HEIGHT 40
#define BUZZ_SPEED -5.0
#define BUZZ_FRAME_DELAY 140
#define BUZZ_CURRENT_FRAME 0
#define BUZZ_LIFE_DELTA -1
#define BUZZ_RING_DELTA -2

Sprite create_buzz_enemy(SDL_Renderer* renderer);
Sprite initialize_buzz(SDL_Renderer* renderer, Frames frames);

#endif
