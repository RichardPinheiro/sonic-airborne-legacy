#ifndef AUDIO_H
#define AUDIO_H

#include "game.h"

typedef enum {
    COLLISION_BAT,
    COLLISION_BUZZ_BEE,
    COLLISION_FLAME,
    COLLISION_PARROT,
    STAGE_1_THEME,
    STAGE_2_THEME,
    STAGE_3_THEME,
    EXTRA_LIFE, 
    GAME_OVER
} AudioID;

typedef struct {
    AudioID id;
    const char* path;
    Mix_Chunk* sound;
    bool is_music;
} AudioAsset;

#endif