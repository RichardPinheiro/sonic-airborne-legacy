#ifndef LIFE_H
#define LIFE_H

#include "game.h"

#define LIFE_WIDTH 50
#define LIFE_HEIGHT 50
#define LIFE_SPEED -8.0
#define LIFE_FRAME_DELAY 100
#define LIFE_CURRENT_FRAME 1
#define LIFE_DELTA 1

Sprite create_life(SDL_Renderer* renderer);
Sprite initialize_life(SDL_Renderer* renderer, Frames frames);

#endif
