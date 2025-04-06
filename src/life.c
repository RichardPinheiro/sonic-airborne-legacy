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
    const char* frame_paths[] = {
        "assets/sprites/extra_lives/life_1.png",
        "assets/sprites/extra_lives/life_2.png"
    };
    size_t frames_length = sizeof(frame_paths) / sizeof(frame_paths[0]);
    Frames frames = {
        frame_paths,
        frames_length,
        LIFE_FRAME_DELAY,
        malloc(sizeof(SDL_Texture*) * frames_length),
        malloc(sizeof(int) * frames_length),
        malloc(sizeof(int) * frames_length)
    };
    if (!frames.texture || !frames.widths || !frames.heights) {
        fprintf(stderr, "Failed to allocate memory for Frames resources.\n");
        exit(EXIT_FAILURE);
    }
    load_texture(&frames, renderer);
    return initialize_life(frames);
}

/**
 * @brief Initializes a life sprite with specified animation frames and properties.
 *
 * This function sets up a life sprite by assigning its type, effect, dimensions,
 * position, speed, and animation frames.
 *
 * @param frames A Frames structure containing the animation frames and related
 *               information for the life sprite.
 * @return A Sprite structure representing the initialized life with its
 *         properties and textures.
 */
Sprite initialize_life(Frames frames) {
    Sprite life;
    life.type = LIFE;
    life.effects.effect_type = LIFE_EFFECT;
    life.effects.life_delta = LIFE_DELTA;
    life.scale = LIFE_ZOOM_SCALE;
    life.collision_state = COLLISION_NONE;
    life.width = frames.widths[life.collision_state];
    life.height = frames.heights[life.collision_state];
    life.x = WINDOW_WIDTH;
    life.y = get_random_y_position(&life);
    life.speed = LIFE_SPEED;
    life.current_frame = LIFE_CURRENT_FRAME;
    life.animation_accumulator = 0;
    life.frames = frames;
    return life;
}
