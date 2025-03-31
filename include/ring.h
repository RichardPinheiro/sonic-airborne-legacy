#ifndef RING_H
#define RING_H

#include "game.h"

#define RING_SPEED -6.0
#define RING_FRAME_DELAY 100
#define RING_CURRENT_FRAME 1
#define RING_DELTA 1
#define RING_ZOOM_SCALE 0.2f

Sprite create_ring(SDL_Renderer* renderer);
Sprite initialize_ring(SDL_Renderer* renderer, Frames frames);

#endif
