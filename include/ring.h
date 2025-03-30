#ifndef RING_H
#define RING_H

#include "game.h"

#define RING_WIDTH 50
#define RING_HEIGHT 50
#define RING_SPEED -6.0
#define RING_FRAME_DELAY 100
#define RING_CURRENT_FRAME 1
#define RING_EFFECT_DELTA 1


Sprite create_ring(SDL_Renderer* renderer);
Sprite initialize_ring(SDL_Renderer* renderer, Frames frames);

#endif
