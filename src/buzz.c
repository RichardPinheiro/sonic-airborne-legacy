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

    size_t frames_length = sizeof(buzz_frame_paths) / sizeof(buzz_frame_paths[0]);

    Frames buzz_frames = {
        buzz_frame_paths,
        frames_length,
        BUZZ_FRAME_DELAY,
        malloc(sizeof(SDL_Texture*) * frames_length),
        .widths = malloc(sizeof(int) * frames_length),
        .heights = malloc(sizeof(int) * frames_length)
    };

    for (size_t i = 0; i < frames_length; i++) {
        SDL_Surface* surface = IMG_Load(buzz_frame_paths[i]);
        if (!surface) {
            printf("Failed to load %s: %s\n", buzz_frame_paths[i], IMG_GetError());
            exit(EXIT_FAILURE);
        }

        buzz_frames.widths[i] = surface->w;
        buzz_frames.heights[i] = surface->h;
        SDL_FreeSurface(surface);

        buzz_frames.texture[i] = IMG_LoadTexture(renderer, buzz_frame_paths[i]);
        if (!buzz_frames.texture[i]) {
            printf("Texture creation failed: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }

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
    create_texture(renderer, &buzz);
    return buzz;
}
