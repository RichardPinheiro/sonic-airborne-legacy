#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "game.h"

#define GAME_OVER_SPEED -2.0
#define GAME_OVER_ZOOM_SCALE 1.0f
#define GAME_OVER_FRAME_DELAY 0
#define GAME_OVER_CURRENT_FRAME 0
#define GAME_OVER_INITIAL_X (WINDOW_WIDTH / 2.0f)
#define GAME_OVER_INITIAL_Y 900
#define GAME_OVER_TARGET_Y (WINDOW_HEIGHT / 2.0f)

typedef struct {
    bool is_active;
} GameOverState;

extern GameOverState game_over_state;

Sprite create_game_over(SDL_Renderer* renderer);
Sprite initialize_game_over(Frames frames);
void game_over_motion(Sprite *game_over, Uint32 delta_time);

#endif
