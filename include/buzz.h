#ifndef BUZZ_H
#define BUZZ_H

#include "game.h"

#define BUZZ_SPEED -5.0
#define BUZZ_FRAME_DELAY 130
#define BUZZ_CURRENT_FRAME 0
#define BUZZ_LIFE_DELTA -1
#define BUZZ_RING_DELTA -2
#define BUZZ_ZOOM_SCALE 1.2f

Sprite create_buzz_enemy(SDL_Renderer* renderer);
Sprite initialize_buzz(Frames frames);

#endif
