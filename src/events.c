#include "game.h"

/**
 * @brief Initializes the event queue by setting the head and tail pointers to 0.
 *
 * This function initializes the event queue by setting the head and tail pointers to 0.
 * The queue is initially empty, and the head and tail pointers are used to enqueue and
 * dequeue events.
 *
 * @return void
 */
void initialize_event_queue(void) {
    global_queue.head = 0;
    global_queue.tail = 0;
}

/**
 * @brief Listens for and processes events in the event queue.
 *
 * This function continuously checks the event queue for new events. When an event is found,
 * it is dequeued and processed based on its type. The function handles different event types
 * by calling the appropriate event handler functions.
 *
 * @param queue A pointer to the EventQueue where events are stored and processed.
 */
void event_listener(EventQueue* queue) {
    while(!is_queue_empty(queue)) {
        GameEvent event = dequeue_event(queue);
        switch(event.type) {
            case EVENT_LIFE_CHANGED: handle_life_event(event); break;
            case EVENT_RINGS_CHANGED: handle_rings_event(event); break;
            case EVENT_SOUND_EFFECT: handle_sfx_event(event); break;
            case EVENT_MUSIC_PLAY: handle_music_event(event); break;
            case EVENT_STOP_AUDIO: handle_stop_audio_event(); break;
            case EVENT_GAME_OVER: handle_game_over_event(); break;
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

/**
 * @brief Checks if the event queue is empty.
 *
 * This function checks if the event queue is empty by comparing the head and tail pointers.
 * If the head and tail pointers are equal, it means the queue is empty.
 *
 * @param queue A pointer to the EventQueue to be checked.
 * @return true if the event queue is empty, false otherwise.
 */
bool is_queue_empty(const EventQueue* queue) {
    return queue->head == queue->tail;
}

/**
 * @brief Handles sound effect events based on the event's payload.
 *
 * This function processes sound effect events by checking the event's payload and
 * playing the corresponding sound effect using the `play_sound` function.
 *
 * @param event The GameEvent containing the sound effect information.
 * @param event.payload.sfx.id The identifier of the sound effect to be played.
 *
 * @return void
 */
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

/**
 * @brief Handles music event based on the event's payload.
 *
 * This function processes music events by checking the event's payload and
 * playing the corresponding music using the `play_music` function.
 *
 * @param event The GameEvent containing the music information.
 * @param event.payload.music.id The identifier of the music to be played.
 * @param event.payload.music.loop A boolean indicating whether the music should loop.
 *
 * @return void
 */
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

/**
 * @brief Handles the stop audio event by calling the stop_audio function.
 *
 * This function is responsible for stopping any currently playing audio.
 * It calls the stop_audio function to achieve this.
 *
 * @return void
 *
 * @see stop_audio
 */
void handle_stop_audio_event(void) {
    stop_audio();
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

/**
 * @brief Handles life-related events, updating the target sprite's life.
 *
 * This function processes life-related events by updating the target
 * sprite's life based on the life delta value from the source sprite's effects.
 * The life value is clamped to a minimum of 0. If the target sprite's
 * life drops to or below 0, the game over start event is emitted.
 *
 * @param event The GameEvent containing the collision information and source/target sprites.
 * @param event.payload.collision.source A pointer to the source sprite involved in the collision.
 * @param event.payload.collision.target A pointer to the target sprite involved in the collision.
 * @param event.payload.collision.source->effects.life_delta The life delta value to be applied to the target sprite's life.
 *
 * @return void
 *
 * @see emit_game_over_start
 */
void handle_life_event(GameEvent event) {
    Sprite* source = event.payload.collision.source;
    Sprite* target = event.payload.collision.target;
    target->life = MAX(target->life + source->effects.life_delta, 0);
    if (target->life <= 0) emit_game_over_start();
}

/**
 * @brief Handles ring-related events, updating the target sprite's rings.
 *
 * This function processes ring-related events by updating the target sprite's rings based on the ring delta
 * value from the source sprite's effects. The rings value is clamped to a minimum of 0.
 *
 * @param event The GameEvent containing the collision information and source/target sprites.
 * @param event.payload.collision.source A pointer to the source sprite involved in the collision.
 * @param event.payload.collision.target A pointer to the target sprite involved in the collision.
 * @param event.payload.collision.source->effects.ring_delta The ring delta value to be applied to the target sprite's rings.
 *
 * @return void
 */
void handle_rings_event(GameEvent event) {
    Sprite* source = event.payload.collision.source;
    Sprite* target = event.payload.collision.target;
    target->rings = MAX(target->rings + source->effects.ring_delta, 0);
}

/**
 * @brief Handles the game over start event by setting the game over state
 *        and emitting stop audio and game over music events.
 *
 * This function is responsible for setting the game over state to active,
 * emitting a stop audio event to stop any currently playing audio,
 * and emitting a game over music event to play the game over music.
 *
 * @return void
 *
 * @see emit_stop_audio
 * @see emit_music
 * @see game_over_state
 */
void handle_game_over_event(void) {
    game_over_state.is_active = true;
    emit_stop_audio();
    emit_music(MUSIC_GAME_OVER, false);
}
