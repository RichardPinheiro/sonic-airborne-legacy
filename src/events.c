#include "game.h"

void initialize_event_queue(void) {
    global_queue.head = 0;
    global_queue.tail = 0;
}

void event_listener(EventQueue* queue) {
    while(!is_queue_empty(queue)) {
        GameEvent event = dequeue_event(queue);
        switch(event.type) {
            case EVENT_LIFE_CHANGED: handle_life_event(event); break;
            case EVENT_RINGS_CHANGED: handle_rings_event(event); break;
            case EVENT_SOUND_EFFECT: handle_sfx_event(event); break;
            case EVENT_MUSIC_PLAY: handle_music_event(event); break;
            case EVENT_GAME_OVER_START: handle_game_over_start_event(event); break;
            case EVENT_GAME_OVER_ANIMATION: handle_game_over_animation_event(event); break;
            case EVENT_GAME_OVER_COMPLETE: handle_game_over_complete_event(event); break;
            case EVENT_GAME_RESET: handle_game_reset_event(event); break;
            case EVENT_DELAYED_RESET: handle_delayed_reset_event(event); break;
        }
    }
}

/**
 * @brief Adds an event to the event queue if there is space available.
 *
 * This function places a new event into the event queue, advancing the head
 * of the queue. If the queue is full, the event is not added.
 *
 * @param queue A pointer to the EventQueue where the event will be added.
 * @param event The GameEvent to be added to the queue.
 */
void queue_event(EventQueue* queue, GameEvent event) {
    if((queue->head + 1) % MAX_EVENTS != queue->tail) {
        queue->events[queue->head] = event;
        queue->head = (queue->head + 1) % MAX_EVENTS;
    }
}

/**
 * @brief Removes and returns the next event from the event queue.
 *
 * This function retrieves the event at the front of the queue and advances
 * the tail of the queue. If the queue is empty, it returns a default event.
 *
 * @param queue A pointer to the EventQueue from which the event will be dequeued.
 * @return The GameEvent at the front of the queue, or a default event if the queue is empty.
 */
GameEvent dequeue_event(EventQueue* queue) {
    if(is_queue_empty(queue)) return (GameEvent){0};
    GameEvent event = queue->events[queue->tail];
    queue->tail = (queue->tail + 1) % MAX_EVENTS;
    return event;
}

bool is_queue_empty(const EventQueue* queue) {
    return queue->head == queue->tail;
}

void handle_sfx_event(GameEvent event) {
    switch(event.payload.sfx.id) {
        case SFX_COLLISION_BUZZ:
        case SFX_COLLISION_BEE:
        case SFX_COLLISION_BAT:
        case SFX_COLLISION_FLAME:
        case SFX_COLLISION_PARROT:
        case SFX_COLLISION_RING:
        case SFX_COLLISION_LIFE:
            play_sound(event.payload.sfx.id);
            break;
    }
}

void handle_music_event(GameEvent event) {
    switch(event.payload.music.id) {
        case MUSIC_STAGE_1:
        case MUSIC_STAGE_2:
        case MUSIC_STAGE_3:
        case MUSIC_GAME_OVER:
            play_music(event.payload.music.id, event.payload.music.loop);
            break;
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

void handle_life_event(GameEvent event) {
    Sprite* source = event.payload.collision.source;
    Sprite* target = event.payload.collision.target;
    target->life = MAX(target->life + source->effects.life_delta, 0);
    if (target->life <= 0) emit_game_over_start();
}

void handle_rings_event(GameEvent event) {
    Sprite* source = event.payload.collision.source;
    Sprite* target = event.payload.collision.target;
    target->rings = MAX(target->rings + source->effects.ring_delta, 0);
}

void handle_game_over_start_event(GameEvent event) {
    GameOverAnimation animation = {
        .current_y = WINDOW_HEIGHT,
        .target_y = GAME_OVER_TARGET_Y_POS,
        .speed = GAME_OVER_ANIM_SPEED,
        .last_update = SDL_GetTicks()
    };
    emit_game_over_animation(animation);
    emit_music(MUSIC_GAME_OVER, false);
}

void handle_game_over_animation_event(GameEvent event) {
    GameOverAnimation animation = event.payload.game_over.animation;
    if(animation.current_y <= animation.target_y) {
        emit_game_over_complete();
        return;
    }
    Uint32 now = SDL_GetTicks();
    Uint32 delta_time = now - animation.last_update;
    float time_scale_factor = get_time_scale_factor(delta_time);

    animation.current_y -= animation.speed * time_scale_factor;
    animation.last_update = now;

    game_over_render_state.y = animation.current_y;
    game_over_render_state.is_active = true;

    emit_game_over_animation(animation);
}

void handle_game_over_complete_event(GameEvent event) {
    GameOverAnimation animation = event.payload.game_over.animation;
    game_over_render_state.y = animation.target_y;
    game_over_render_state.is_active = true;
    emit_delay();
}

void handle_delayed_reset_event(GameEvent event) {
    if(SDL_GetTicks() >= event.timestamp) {
        emit_game_reset();
    } else {
        queue_event(&global_queue, event); // Requeue until ready
    }
}

void handle_game_reset_event(GameEvent event) {
    game_over_render_state.is_active = false;
    game_over_render_state.y = WINDOW_HEIGHT;
}
