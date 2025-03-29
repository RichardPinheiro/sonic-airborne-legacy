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
    const char* buzz_frame_paths[] = {
        "assets/sprites/enemies/buzz/buzz_1.png",
        "assets/sprites/enemies/buzz/buzz_2.png"
    };

    int frames_length = sizeof(buzz_frame_paths) / sizeof(buzz_frame_paths[0]);

    SDL_Texture** buzz_texture = malloc(sizeof(SDL_Texture*) * frames_length);
    if (!buzz_texture) {
        printf("Memory allocation failed for buzz_texture!\n");
        exit(1);
    }

    Frames buzz_frames = {
        buzz_frame_paths,
        frames_length,
        BUZZ_FRAME_DELAY,
        buzz_texture
    };

    return initialize_buzz(renderer, buzz_frames);
}

/**
 * @brief Initializes a buzz enemy sprite with specified frames and renderer.
 *
 * This function sets up a buzz enemy sprite, assigning its type, dimensions,
 * position, speed, current frame, and animation frames. It also creates the
 * texture for the sprite using the provided renderer.
 *
 * @param renderer A pointer to the SDL_Renderer used to render the sprite.
 * @param frames A Frames structure containing the animation frames and related data.
 * @return A Sprite structure representing the initialized buzz enemy.
 */
Sprite initialize_buzz(SDL_Renderer* renderer, Frames frames) {
    Sprite buzz;
    buzz.type = BUZZ;
    buzz.width = BUZZ_WIDTH;
    buzz.height = BUZZ_HEIGHT;
    buzz.x = WINDOW_WIDTH;
    buzz.y = set_random_position(&buzz);
    buzz.speed = BUZZ_SPEED;
    buzz.current_frame = ENEMY_CURRENT_FRAME;
    buzz.animation_accumulator = 0;
    buzz.frames = frames;
    create_texture(renderer, &buzz);
    return buzz;
}
