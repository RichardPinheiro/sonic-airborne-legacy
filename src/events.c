#include "game.h"

void initialize_event_queue(void) {
    global_queue.head = 0;
    global_queue.tail = 0;
}

void process_events(EventQueue* queue) {
    while(!is_queue_empty(queue)) {
        GameEvent event = dequeue_event(queue);
        switch(event.type) {
            case EVENT_LIFE_CHANGED: handle_life_events(event); break;
            case EVENT_SOUND_EFFECT: handle_sfx_events(event); break;
            case EVENT_MUSIC_PLAY: handle_music_events(event); break;
        }
    }
}

void queue_event(EventQueue* queue, GameEvent event) {
    if((queue->head + 1) % MAX_EVENTS != queue->tail) {
        queue->events[queue->head] = event;
        queue->head = (queue->head + 1) % MAX_EVENTS;
    }
}

GameEvent dequeue_event(EventQueue* queue) {
    if(is_queue_empty(queue)) return (GameEvent){0};
    GameEvent event = queue->events[queue->tail];
    queue->tail = (queue->tail + 1) % MAX_EVENTS;
    return event;
}

bool is_queue_empty(const EventQueue* queue) {
    return queue->head == queue->tail;
}

void handle_sfx_events(GameEvent event) {
    switch(event.payload.sfx.id) {
        case SFX_COLLISION_BUZZ:
        case SFX_COLLISION_BEE:
        case SFX_COLLISION_BAT:
        case SFX_COLLISION_FLAME:
        case SFX_COLLISION_PARROT:
        case SFX_EXTRA_LIFE:
            play_sound(event.payload.sfx.id);
            break;
    }
}

void handle_music_events(GameEvent event) {
    switch(event.payload.music.id) {
        case MUSIC_STAGE_1: play_music(event.payload.music.id, event.payload.music.loop); break;
        case MUSIC_STAGE_2: play_music(event.payload.music.id, event.payload.music.loop); break;
        case MUSIC_STAGE_3: play_music(event.payload.music.id, event.payload.music.loop); break;
    }
}

void handle_background_events(GameEvent event) {
    if(event.type == EVENT_BACKGROUND_CHANGE) {
        switch(event.payload.stage.index) {
            // case 0: load_background("stage1.png"); break;
            // case 1: load_background("stage2.png"); break;
            // case 2: load_background("stage3.png"); break;
        }
    }
}

void handle_life_events(GameEvent event) {
    printf("handle_life_events \n");
    printf("current_life: %d\n", event.payload.life.current += event.payload.life.delta);
    // if(event.type == EVENT_LIFE_CHANGED) {
    //     // handle_life_decrement();
    //     // update_life_display(event.current_life);
        
    //     if(event.current_life <= 0) {
    //         // queue_event(&global_queue, (GameEvent){EVENT_GAME_OVER});
    //     }
    // }
}
