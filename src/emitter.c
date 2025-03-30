#include "game.h"

EmitterResult emit_life_change(Sprite* source, Sprite* target) {
    GameEvent life_event = {
        .type = EVENT_LIFE_CHANGED,
        .payload.collision.source = source,
        .payload.collision.target = target
    };
    emit_event(life_event);
    return EMITTER_SUCCESS;
}

EmitterResult emit_sfx(AudioID sfx_id) {
    GameEvent sfx_event = {
        .type = EVENT_SOUND_EFFECT,
        .payload.sfx.id = sfx_id
    };
    emit_event(sfx_event);
    return EMITTER_SUCCESS;
}

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

EmitterResult emit_game_over(void) {
    GameEvent game_over_event = {
        .type = EVENT_GAME_OVER
    };
    emit_event(game_over_event);
    return EMITTER_SUCCESS;
}

static void emit_event(const GameEvent event) {
    queue_event(&global_queue, event);
}