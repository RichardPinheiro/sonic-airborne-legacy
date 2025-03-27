#ifndef SPRITE_H
#define SPRITE_H

#include "game.h"

#define NORMALIZATION_FACTOR 16.0f
#define TARGET_FRAME_TIME 16

typedef struct Frames {
    const char** paths;
    int length;
    Uint32 delay;
    SDL_Texture** texture;
} Frames;

typedef enum {
    TYPE_PLAYER,
    TYPE_ENEMY,
    TYPE_COIN
} SpriteType;

typedef enum {
    COLLISION_NONE,
    COLLISION_ENTER,
    COLLISION_STAY,
    COLLISION_EXIT
} CollisionState;

typedef struct Sprite {
    float x, y;
    int width, height;
    int life;
    float speed;
    float velocity_x, velocity_y;
    float acceleration, friction;
    int current_frame;
    float hover_amplitude;
    float hover_frequency;
    float boundary_left, boundary_right;
    float boundary_top, boundary_bottom;
    Uint32 hover_start_time;
    Uint32 animation_accumulator;
    Frames frames;
    SpriteType type;
    CollisionState collision_state;
} Sprite;

void create_texture(SDL_Renderer* renderer, Sprite *sprite);
void sprite_animation(Sprite *sprite, Uint32 delta_time);
void sprite_render(Sprite *sprite, SDL_Renderer* renderer);
int set_random_position(Sprite *sprite);
void free_sprite_frames(Sprite *sprite);
float get_time_scale_factor(Uint32 delta_time);
bool check_collision(Sprite *sprite_a, Sprite *sprite_b);
void update_sprite_boundaries(Sprite *sprite);
void update_collision_states(Sprite *sonic, Sprite **sprites, int sprites_length);
void handle_collisions(Sprite *sonic, Sprite **sprites, int sprites_length) ;

#endif