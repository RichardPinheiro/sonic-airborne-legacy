#include "game.h"

/**
 * @brief Creates a life sprite with animation frames.
 *
 * This function initializes a life sprite by loading its animation frames
 * and setting up the necessary textures using the provided renderer.
 *
 * @param renderer A pointer to the SDL_Renderer used to render the sprite.
 *                 It is used to create textures for the life's animation frames.
 * @return A Sprite structure representing the initialized life with its
 *         animation frames and textures.
 */
Sprite create_life(SDL_Renderer* renderer) {
    const char* life_frame_paths[] = {
        "assets/sprites/extra_lives/life_1.png",
        "assets/sprites/extra_lives/life_2.png"
    };

    size_t frames_length = sizeof(life_frame_paths)/sizeof(life_frame_paths[0]);

    Frames life_frames = {
        life_frame_paths,
        frames_length,
        LIFE_FRAME_DELAY,
        malloc(sizeof(SDL_Texture*) * frames_length),
        .widths = malloc(sizeof(int) * frames_length),
        .heights = malloc(sizeof(int) * frames_length)
    };

    for (size_t i = 0; i < frames_length; i++) {
        SDL_Surface* surface = IMG_Load(life_frame_paths[i]);
        if (!surface) {
            printf("Failed to load %s: %s\n", life_frame_paths[i], IMG_GetError());
            exit(EXIT_FAILURE);
        }

        life_frames.widths[i] = surface->w;
        life_frames.heights[i] = surface->h;
        SDL_FreeSurface(surface);

        life_frames.texture[i] = IMG_LoadTexture(renderer, life_frame_paths[i]);
        if (!life_frames.texture[i]) {
            printf("Texture creation failed: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }

    return initialize_life(renderer, life_frames);
}

/**
 * @brief Initializes a life sprite with specified animation frames and properties.
 *
 * This function sets up a life sprite by assigning its type, effect, dimensions,
 * position, speed, and animation frames. It also creates the necessary textures
 * for rendering the sprite using the provided renderer.
 *
 * @param renderer A pointer to the SDL_Renderer used to render the sprite.
 *                 It is used to create textures for the life's animation frames.
 * @param frames A Frames structure containing the animation frames and related
 *               information for the life sprite.
 * @return A Sprite structure representing the initialized life with its
 *         properties and textures.
 */
Sprite initialize_life(SDL_Renderer* renderer, Frames frames) {
    Sprite life;
    life.type = LIFE;
    life.effects.effect_type = LIFE_EFFECT;
    life.effects.life_delta = LIFE_DELTA;
    life.scale = LIFE_ZOOM_SCALE;
    life.width = frames.widths[0];
    life.height = frames.heights[0];
    life.x = WINDOW_WIDTH;
    life.y = set_random_y_position(&life);
    life.speed = LIFE_SPEED;
    life.current_frame = LIFE_CURRENT_FRAME;
    life.collision_state = COLLISION_NONE;
    life.animation_accumulator = 0;
    life.frames = frames;
    create_texture(renderer, &life);
    return life;
}
