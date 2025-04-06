#include "game.h"

static AudioAsset audio_registry[AUDIO_COUNT];

/**
 * @brief Initializes the audio system by loading all audio assets defined in the audio registry.
 *
 * This function reads the audio registry definition file (audio_registry.def) and loads each audio asset
 * into the audio registry array. The audio assets can be either music or sound effects.
 *
 * @param void This function does not take any parameters.
 *
 * @return void This function does not return any value.
 */
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

/**
 * @brief Plays a sound effect from the audio registry.
 *
 * This function uses the SDL_mixer library to play a specific sound effect from the audio registry.
 * The sound effect is identified by its AudioID, which corresponds to its position in the audio registry array.
 *
 * @param id The AudioID of the sound effect to be played.
 *
 * @return void This function does not return any value.
 */
void play_sound(AudioID id) {
    Mix_PlayChannel(-1, audio_registry[id].sound, 0);
}

/**
 * @brief Plays a music track from the audio registry.
 *
 * This function uses the SDL_mixer library to play a specific music track from the audio registry.
 * The music track is identified by its AudioID, which corresponds to its position in the audio registry array.
 *
 * @param id The AudioID of the music track to be played.
 * @param loop A boolean value indicating whether the music track should be played in a loop.
 *
 * @return void This function does not return any value.
 */
void play_music(AudioID id, bool loop) {
    Mix_PlayMusic(audio_registry[id].music, loop ? -1 : 0);
}

/**
 * @brief Stops all currently playing audio channels and music tracks.
 *
 * This function uses the SDL_mixer library to halt all currently playing audio channels and music tracks.
 * It effectively stops all sound effects and music from playing.
 *
 * @param void This function does not take any parameters.
 *
 * @return void This function does not return any value.
 */
void stop_audio(void) {
    Mix_HaltChannel(-1);
    Mix_HaltMusic();
}

/**
 * @brief Sets the volume of all currently playing audio channels and music tracks.
 *
 * This function uses the SDL_mixer library to set the volume of all currently playing audio channels and music tracks.
 * The volume is specified as an integer value between 0 (silent) and 128 (full volume).
 *
 * @param volume An integer value representing the desired volume level. Must be within the range [0, 128].
 *
 * @return void This function does not return any value.
 */
void set_volume(int volume) {
    Mix_Volume(-1, volume);
}

/**
 * @brief Retrieves the appropriate sound effect for a given sprite collision.
 *
 * This function takes a SpriteType as input and returns the corresponding AudioID for the collision sound effect.
 * The function uses a switch statement to map each SpriteType to its respective collision sound effect.
 * If the input SpriteType does not match any of the cases, the function returns the default sound effect (SFX_COLLISION_BUZZ).
 *
 * @param type The SpriteType for which the collision sound effect is required.
 *
 * @return The AudioID of the collision sound effect corresponding to the input SpriteType.
 */
AudioID get_collision_sound(SpriteType type) {
    switch(type) {
        case BUZZ: return SFX_COLLISION_BUZZ;
        case BEE: return SFX_COLLISION_BEE;
        case BAT: return SFX_COLLISION_BAT;
        case FLAME: return SFX_COLLISION_FLAME;
        case PARROT: return SFX_COLLISION_PARROT;
        case RING: return SFX_COLLISION_RING;
        case LIFE: return SFX_COLLISION_LIFE;
        default: return SFX_COLLISION_BUZZ;
    }
}

/**
 * @brief Cleans up and frees all loaded audio assets.
 *
 * This function is responsible for freeing all loaded audio assets, both music and sound effects,
 * from the audio registry array. It first emits a stop_audio event to ensure that no audio is playing
 * before proceeding with the cleanup process. Then, it iterates through the audio registry array,
 * checking the type of each audio asset (music or sound effect) and freeing the corresponding SDL_mixer
 * resource. After freeing the resource, it sets the pointer to NULL to prevent any dangling pointers.
 *
 * @param void This function does not take any parameters.
 *
 * @return void This function does not return any value.
 */
void audio_cleanup(void) {
    emit_stop_audio();
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
