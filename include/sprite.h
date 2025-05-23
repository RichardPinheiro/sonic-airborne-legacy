#ifndef SPRITE_H
#define SPRITE_H

#include "game.h"

#define NORMALIZATION_FACTOR 16.0f
#define TARGET_FRAME_TIME 16

typedef struct Frames {
    const char** paths;
    size_t length;
    Uint32 delay;
    SDL_Texture** texture;
    int* widths;
    int* heights;
} Frames;

typedef enum {
    PLAYER,
    BUZZ,
    BEE,
    BAT,
    FLAME,
    PARROT,
    RING,
    LIFE,
    GAME_OVER
} SpriteType;

typedef enum {
    COLLISION_NONE,
    COLLISION_ENTER,
    COLLISION_STAY,
    COLLISION_EXIT
} CollisionState;

typedef enum {
    DAMAGE_EFFECT,
    LIFE_EFFECT,
    SCORE_EFFECT,
    RING_EFFECT
} EffectType;

typedef struct {
    EffectType effect_type;
    int16_t life_delta;
    int16_t ring_delta;
} Effects;


typedef struct Sprite {
    float x, y, target_y;
    int width, height;
    int life, rings;
    float scale, speed;
    float velocity_x, velocity_y;
    float acceleration, friction;
    float hover_amplitude, hover_frequency;
    float boundary_left, boundary_right;
    float boundary_top, boundary_bottom;
    size_t current_frame;
    Uint32 hover_start_time;
    Uint32 animation_accumulator;
    Frames frames;
    SpriteType type;
    CollisionState collision_state;
    Effects effects;
} Sprite;

void load_texture(Frames* frames, SDL_Renderer* renderer);
void sprite_animation(Sprite *sprite, Uint32 delta_time);
void sprite_motion(Sprite *sprite, Uint32 delta_time);
void sprite_render(Sprite *sprite, SDL_Renderer* renderer);
int get_random_y_position(const Sprite *sprite);
float get_vertical_center_offset(const Sprite* sprite);
void free_sprite_frames(Sprite *sprite);
float get_time_scale_factor(Uint32 delta_time);
bool check_collision(Sprite *sprite_a, Sprite *sprite_b);
void update_sprite_boundaries(Sprite *sprite);
void update_collision_states(Sprite *sonic, Sprite **sprites, size_t sprites_length);
void handle_collisions(Sprite *sonic, Sprite **sprites, size_t sprites_length);
void handle_collision_enter(Sprite *sprite, Sprite *sonic);
void handle_collision_stay(Sprite *sprite, Sprite *sonic);
void handle_collision_exit(Sprite *sprite, Sprite *sonic);
void apply_penalties(Sprite* source, Sprite* target);
void apply_bonus(Sprite* source, Sprite* target);
void apply_Life(Sprite* source, Sprite* target);

#endif
