#include "game.h"

AudioAsset audio_registry[] = {

    { COLLISION_BAT, "assets/sounds/collisions/bat.mp3", NULL, false },
    { COLLISION_BUZZ_BEE, "assets/sounds/collisions/buzz_and_bee.mp3", NULL, false },
    { COLLISION_FLAME, "assets/sounds/collisions/flame.mp3", NULL, false },
    { COLLISION_PARROT, "assets/sounds/collisions/parrot.mp3", NULL, false },
    { STAGE_1_THEME, "assets/sounds/stages/stage_1.mp3", NULL, true },
    { STAGE_2_THEME, "assets/sounds/stages/stage_2.mp3", NULL, true },
    { STAGE_3_THEME, "assets/sounds/stages/stage_3.mp3", NULL, true },
    { EXTRA_LIFE, "assets/sounds/extra_life.mp3", NULL, false },
    { GAME_OVER, "assets/sounds/game_over.mp3", NULL, false }
};

#define AUDIO_REGISTRY_SIZE (sizeof(audio_registry)/sizeof(audio_registry[0]));