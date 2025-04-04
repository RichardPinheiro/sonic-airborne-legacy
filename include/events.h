#ifndef EVENTS_H
#define EVENTS_H

#include "game.h"

#define MAX_EVENTS 20
#define GAME_OVER_ANIM_SPEED  200.0f
#define GAME_OVER_TARGET_Y_POS (WINDOW_HEIGHT / 2)

typedef enum {
    EVENT_LIFE_CHANGED,
    EVENT_SCORE_CHANGED,
    EVENT_RINGS_CHANGED,
    EVENT_STAGE_CHANGED,
    EVENT_MUSIC_PLAY,
    EVENT_SOUND_EFFECT,
    EVENT_BACKGROUND_CHANGE,
    EVENT_SCREEN_SHAKE,
    EVENT_GAME_OVER_START,
    EVENT_GAME_OVER_ANIMATION,
    EVENT_GAME_OVER_COMPLETE,
    EVENT_GAME_RESET,
    EVENT_DELAYED_RESET
} GameEventType;

typedef struct {
    bool is_active;
    float y;
    SDL_Texture* texture;
    int width;
    int height;
} GameOverRenderState;

extern GameOverRenderState game_over_render_state;

typedef struct {
    float current_y;
    float target_y;
    float speed;
    Uint32 last_update;
} GameOverAnimation;

typedef struct {
    GameEventType type;
    Uint32 timestamp;
    union {
        struct { Sprite* source; Sprite* target; } collision;
        struct { AudioID id; } sfx;
        struct { AudioID id;  bool loop; } music;
        struct { int index; } stage;
        struct { GameOverAnimation animation; } game_over;
    } payload;
} GameEvent;

typedef struct {
    GameEvent events[MAX_EVENTS];
    int head;
    int tail;
} EventQueue;

extern EventQueue global_queue;

void initialize_event_queue(void);
void event_listener(EventQueue* queue);
void queue_event(EventQueue* queue, GameEvent event);
GameEvent dequeue_event(EventQueue* queue);
bool is_queue_empty(const EventQueue* queue);
void handle_sfx_event(GameEvent event);
void handle_music_event(GameEvent event);
void handle_background_events(GameEvent event);
void handle_life_event(GameEvent event);
void handle_rings_event(GameEvent event);
void handle_game_over_start_event(GameEvent event);
void handle_game_over_animation_event(GameEvent event);
void handle_game_over_complete_event(GameEvent event);
void handle_delayed_reset_event(GameEvent event);
void handle_game_reset_event(GameEvent event);

#endif
