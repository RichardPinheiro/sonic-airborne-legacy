#include "game.h"

/**
 * @brief Emits an event indicating a change in life status between two sprites.
 *
 * This function creates and emits a game event that signifies a change in the life
 * status, involving a source and a target sprite.
 *
 * @param source A pointer to the source Sprite involved in the life change event.
 * @param target A pointer to the target Sprite involved in the life change event.
 * @return EmitterResult indicating the success of the event emission.
 */
EmitterResult emit_life_change(Sprite* source, Sprite* target) {
    GameEvent life_event = {
        .type = EVENT_LIFE_CHANGED,
        .payload.collision = {
            .source = source,
            .target = target
        }
    };
    emit_event(life_event);
    return EMITTER_SUCCESS;
}

/**
 * @brief Emits an event indicating a change in rings status between two sprites.
 *
 * This function creates and emits a game event that signifies a change in the rings
 * status, involving a source and a target sprite.
 *
 * @param source A pointer to the source Sprite involved in the rings change event.
 * @param target A pointer to the target Sprite involved in the rings change event.
 * @return EmitterResult indicating the success of the event emission.
 */
EmitterResult emit_rings_change(Sprite* source, Sprite* target) {
    GameEvent rings_event = {
        .type = EVENT_RINGS_CHANGED,
        .payload.collision = {
            .source = source,
            .target = target
        }
    };
    emit_event(rings_event);
    return EMITTER_SUCCESS;
}

/**
 * @brief Emits a sound effect event.
 *
 * This function creates and emits a game event to play a sound effect, based on the provided AudioID.
 *
 * @param sfx_id The identifier for the sound effect to be played.
 * @return EmitterResult indicating the success of the event emission.
 */
EmitterResult emit_sfx(AudioID sfx_id) {
    GameEvent sfx_event = {
        .type = EVENT_SOUND_EFFECT,
        .payload.sfx.id = sfx_id
    };
    emit_event(sfx_event);
    return EMITTER_SUCCESS;
}

/**
 * @brief Emits a music play event.
 *
 * This function creates and emits a game event to play background music. It takes an AudioID and a loop flag,
 * indicating whether the music should be looped.
 *
 * @param music_id The identifier for the music track to be played.
 * @param loop A boolean flag that indicates if the music should loop continuously.
 * @return EmitterResult indicating the success of the event emission.
 */
EmitterResult emit_music(AudioID music_id, bool loop) {
    GameEvent music_event = {
        .type = EVENT_MUSIC_PLAY,
        .payload.music = {
            .id = music_id,
            .loop = loop
        }
    };
    emit_event(music_event);
    return EMITTER_SUCCESS;
}

/**
 * @brief Emits a game over event.
 *
 * This function creates and emits a game event indicating that the game is over.
 *
 * @return EmitterResult indicating the success of the event emission.
 */
EmitterResult emit_game_over_start(void) {
    GameEvent game_over_start_event = {
        .type = EVENT_GAME_OVER_START,
    };
    emit_event(game_over_start_event);
    return EMITTER_SUCCESS;
}

EmitterResult emit_game_over_animation(GameOverAnimation animation) {
    GameEvent game_over_animation_event = {
        .type = EVENT_GAME_OVER_ANIMATION,
        .payload.game_over.animation = animation
    };
    emit_event(game_over_animation_event);
    return EMITTER_SUCCESS;
}

EmitterResult emit_game_over_complete(void) {
    GameEvent game_over_complete_event = {
        .type = EVENT_GAME_OVER_COMPLETE
    };
    emit_event(game_over_complete_event);
    return EMITTER_SUCCESS;
}

EmitterResult emit_delay(void) {
    GameEvent delay_event = {
        .type = EVENT_DELAYED_RESET,
        .timestamp = SDL_GetTicks() + 3000 // 3 second delay
    };
    emit_event(delay_event);
    return EMITTER_SUCCESS;
}

EmitterResult emit_game_reset(void) {
    GameEvent game_reset_event = {
        .type = EVENT_GAME_RESET,
        .timestamp = SDL_GetTicks()
    };
    emit_event(game_reset_event);
    return EMITTER_SUCCESS;
}

/**
 * @brief Queues a game event.
 *
 * This function takes a GameEvent and enqueues it into the global event queue.
 *
 * @param event The GameEvent to be queued.
 */
void emit_event(GameEvent event) {
    queue_event(&global_queue, event);
}
