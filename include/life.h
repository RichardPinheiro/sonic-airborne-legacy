#ifndef LIFE_H
#define LIFE_H

#include "game.h"

#define LIFE_SPEED -7.0
#define LIFE_FRAME_DELAY 190
#define LIFE_CURRENT_FRAME 1
#define LIFE_DELTA 1
#define LIFE_ZOOM_SCALE 0.5f

Sprite create_life(SDL_Renderer* renderer);
Sprite initialize_life(Frames frames);

#endif
