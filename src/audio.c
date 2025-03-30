#include "game.h"

static AudioAsset audio_registry[AUDIO_COUNT];

void audio_initialization(void) {
    #define AUDIO_ENTRY(id, path, is_music) \
        if(is_music) { \
            audio_registry[id].music = Mix_LoadMUS(path); \
        } else { \
            audio_registry[id].sound = Mix_LoadWAV(path); \
        }
    #include "audio_registry.def"
    #undef AUDIO_ENTRY
}

void play_sound(AudioID id) {
    Mix_PlayChannel(-1, audio_registry[id].sound, 0);
}

void play_music(AudioID id, bool loop) {
    Mix_PlayMusic(audio_registry[id].music, loop ? -1 : 0);
}

void stop_music(void) {
    Mix_HaltMusic();
}

void set_volume(int volume) {
    Mix_Volume(-1, volume);
}

AudioID get_collision_sound(SpriteType type) {
    switch(type) {
        case BUZZ: return SFX_COLLISION_BUZZ;
        case BEE: return SFX_COLLISION_BEE;
        case BAT: return SFX_COLLISION_BAT;
        case FLAME: return SFX_COLLISION_FLAME;
        case PARROT: return SFX_COLLISION_PARROT;
        case RING: return SFX_COLLISION_RING;
        default: return SFX_COLLISION_BUZZ;
    }
}

void audio_cleanup(void) {
    Mix_HaltChannel(-1); //Stop all sound effects immediately -1 = all channels
    Mix_HaltMusic();     // Stop music playback
    // Free memory
    for(int i = 0; i < AUDIO_COUNT; i++) {
        if(audio_registry[i].is_music) {
            if(audio_registry[i].music) {
                Mix_FreeMusic(audio_registry[i].music);
                audio_registry[i].music = NULL;
            }
        } else {
            if(audio_registry[i].sound) {
                Mix_FreeChunk(audio_registry[i].sound);
                audio_registry[i].sound = NULL;
            }
        }
    }
}
