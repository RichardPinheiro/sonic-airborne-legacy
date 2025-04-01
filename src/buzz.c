#include "game.h"

/**
 * @brief Creates a Buzz enemy sprite with animation frames and initializes its textures.
 *
 * This function defines frame paths for Buzz's animation, allocates
 * memory for texture pointers. It also initializes the Frames
 * structure and calls initialize_buzz() for final sprite setup.
 *
 * @param renderer SDL_Renderer used for texture creation
 * @return Sprite Fully initialized Buzz enemy sprite
 */
Sprite create_buzz_enemy(SDL_Renderer* renderer) {
    const char* frame_paths[] = {
        "assets/sprites/enemies/buzz/buzz_1.png",
        "assets/sprites/enemies/buzz/buzz_2.png"
    };
    size_t frames_length = sizeof(frame_paths) / sizeof(frame_paths[0]);
    Frames frames = {
        frame_paths,
        frames_length,
        BUZZ_FRAME_DELAY,
        malloc(sizeof(SDL_Texture*) * frames_length),
        malloc(sizeof(int) * frames_length),
        malloc(sizeof(int) * frames_length)
    };
    if (!frames.texture || !frames.widths || !frames.heights) {
        fprintf(stderr, "Failed to allocate memory for Frames resources.\n");
        exit(EXIT_FAILURE);
    }
    load_texture(&frames, renderer);
    return initialize_buzz(frames);
}

/**
 * @brief Initializes a buzz enemy sprite with specified frames.
 *
 * This function sets up a buzz enemy sprite, assigning its type, dimensions,
 * position, speed, current frame, and animation frames.
 *
 * @param frames A Frames structure containing the animation frames and related data.
 * @return A Sprite structure representing the initialized buzz enemy.
 */
Sprite initialize_buzz(Frames frames) {
    Sprite buzz;
    buzz.type = BUZZ;
    buzz.effects.effect_type = DAMAGE_EFFECT;
    buzz.effects.life_delta = BUZZ_LIFE_DELTA;
    buzz.effects.ring_delta = BUZZ_RING_DELTA;
    buzz.scale = BUZZ_ZOOM_SCALE;
    buzz.width = frames.widths[0];
    buzz.height = frames.heights[0];
    buzz.x = WINDOW_WIDTH;
    buzz.y = set_random_y_position(&buzz);
    buzz.speed = BUZZ_SPEED;
    buzz.current_frame = BUZZ_CURRENT_FRAME;
    buzz.collision_state = COLLISION_NONE;
    buzz.animation_accumulator = 0;
    buzz.frames = frames;
    return buzz;
}
