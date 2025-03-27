#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"

#define ENEMY_CURRENT_FRAME 0
#define ENEMY_TYE 0

void enemy_motion(Sprite *enemy, Uint32 delta_time);

#endif