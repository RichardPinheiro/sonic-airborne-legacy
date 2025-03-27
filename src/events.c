#include "game.h"

void initialize_event_queue() {
    global_queue.head = 0;
    global_queue.tail = 0;
}

void process_events(EventQueue* queue) {
    while(!is_queue_empty(queue)) {
        GameEvent event = dequeue_event(queue);
        switch(event.type) {
            case EVENT_LIFE_CHANGED:
                handle_life_events(event);
                break;
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

void handle_audio_events(GameEvent event) {
    switch(event.type) {
        case EVENT_MUSIC_PLAY:
            // play_music(event.audio_key, event.loop);
            break;
        case EVENT_SOUND_EFFECT:
            // play_sound(event.audio_key);
            break;
    }
}

void handle_background_events(GameEvent event) {
    if(event.type == EVENT_BACKGROUND_CHANGE) {
        switch(event.stage_index) {
            // case 0: load_background("stage1.png"); break;
            // case 1: load_background("stage2.png"); break;
            // case 2: load_background("stage3.png"); break;
        }
    }
}

void handle_life_events(GameEvent event) {
    printf("handle_life_events \n");
    printf("current_life: %d\n", event.current_life += event.life_delta);
    // if(event.type == EVENT_LIFE_CHANGED) {
    //     // handle_life_decrement();
    //     // update_life_display(event.current_life);
        
    //     if(event.current_life <= 0) {
    //         // queue_event(&global_queue, (GameEvent){EVENT_GAME_OVER});
    //     }
    // }
}