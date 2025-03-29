#ifndef BUZZ_H
#define BUZZ_H

#include "game.h"

#define BUZZ_WIDTH 90
#define BUZZ_HEIGHT 40
#define BUZZ_SPEED -5.0
#define BUZZ_FRAME_DELAY 140

Sprite create_buzz_enemy(SDL_Renderer* renderer);
Sprite initialize_buzz(SDL_Renderer* renderer, Frames frames);

#endif
