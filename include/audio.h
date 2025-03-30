#ifndef AUDIO_H
#define AUDIO_H

#include "game.h"

typedef enum {
    #define AUDIO_ENTRY(id, path, is_music) id,
    #include "audio_registry.def"
    #undef AUDIO_ENTRY
    AUDIO_COUNT
} AudioID;

typedef struct {
    AudioID id;
    union {
        Mix_Music* music;
        Mix_Chunk* sound;
    };
    bool is_music;
} AudioAsset;

void audio_initialization(void);
void play_sound(AudioID id);
void play_music(AudioID id, bool loop);
void stop_music(void);
void set_volume(int volume);
AudioID get_collision_sound(SpriteType type);
void audio_cleanup(void);

#endif
