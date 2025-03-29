#ifndef SONIC_H
#define SONIC_H

#include "game.h"

#define SONIC_WIDTH 200
#define SONIC_HEIGHT 157
#define SONIC_FRAME_DELAY 100
#define SONIC_CURRENT_FRAME 0
#define SONIC_SPEED 0
#define SONIC_MAX_LIFE 5

Sprite create_sonic(SDL_Renderer* renderer);
Sprite initialize_sonic(SDL_Renderer* renderer, Frames frames);
void sonic_motion(Sprite *sonic, Uint32 delta_time, const Uint8 *keystates);
void watch_player_interactions(Sprite *sonic, const Uint8 *keystates);
void apply_hover_effect(Sprite *sonic);
bool is_arrow_pressed(const Uint8 *keystates);
void apply_friction(Sprite *sonic, float time_scale_factor); 
void update_position(Sprite *sonic, float time_scale_factor);
void check_boundary(Sprite *sonic);
void handle_enemy_collision(Sprite* sonic, Sprite* enemy);
void handle_coin_collision(Sprite* sonic, Sprite* coin);

#endif
