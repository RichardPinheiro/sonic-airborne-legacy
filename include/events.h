#ifndef EVENTS_H
#define EVENTS_H

#include "game.h"

#define MAX_EVENTS 10

typedef enum {
    EVENT_LIFE_CHANGED,
    EVENT_SCORE_CHANGED,
    EVENT_RINGS_CHANGED,
    EVENT_STAGE_CHANGED,
    EVENT_MUSIC_PLAY,
    EVENT_SOUND_EFFECT,
    EVENT_BACKGROUND_CHANGE,
    EVENT_SCREEN_SHAKE,
    EVENT_GAME_OVER
} GameEventType;

typedef struct {
    GameEventType type;
    union {
        struct { Sprite* source; Sprite* target; } collision;
        struct { AudioID id; } sfx;
        struct { AudioID id;  bool loop; } music;
        struct { int index; } stage;
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
void handle_sfx_events(GameEvent event);
void handle_music_events(GameEvent event);
void handle_background_events(GameEvent event);
void handle_life_events(GameEvent event);
void handle_rings_events(GameEvent event);
void handle_game_over_events(GameEvent event);

#endif
