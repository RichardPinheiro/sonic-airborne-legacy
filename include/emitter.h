#ifndef EMITTER_H
#define EMITTER_H

#include "game.h"

typedef enum {
    EMITTER_SUCCESS,
} EmitterResult;

EmitterResult emit_life_change(Sprite* source, Sprite* target);
EmitterResult emit_rings_change(Sprite* source, Sprite* target);
EmitterResult emit_sfx(AudioID sfx_id);
EmitterResult emit_music(AudioID music_id, bool loop);
EmitterResult emit_stop_audio(void);
EmitterResult emit_game_over_start(void);
void emit_event(GameEvent event);

#endif
