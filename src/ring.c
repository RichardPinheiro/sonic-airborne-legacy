#include "game.h"

/**
 * @brief Creates a ring sprite with animation frames.
 *
 * This function initializes a ring sprite by loading its animation frames
 * and setting up the necessary textures using the provided renderer.
 *
 * @param renderer A pointer to the SDL_Renderer used to render the sprite.
 *                 It is used to create textures for the ring's animation frames.
 * @return A Sprite structure representing the initialized ring with its
 *         animation frames and textures.
 */
Sprite create_ring(SDL_Renderer* renderer) {
    const char* ring_frame_paths[] = {
        "assets/sprites/ring/ring_1.png",
        "assets/sprites/ring/ring_2.png",
        "assets/sprites/ring/ring_3.png",
        "assets/sprites/ring/ring_4.png"
    };

    size_t frames_length = sizeof(ring_frame_paths) / sizeof(ring_frame_paths[0]);

    SDL_Texture** ring_texture = malloc(sizeof(SDL_Texture*) * frames_length);
    if (!ring_texture) {
        printf("Memory allocation failed for ring_texture!\n");
        exit(EXIT_FAILURE);
    }

    Frames ring_frames = {
        ring_frame_paths,
        frames_length,
        RING_FRAME_DELAY,
        ring_texture
    };

    return initialize_ring(renderer, ring_frames);
}

/**
 * @brief Initializes a ring sprite with specified animation frames and properties.
 *
 * This function sets up a ring sprite by assigning its type, effect, dimensions,
 * position, speed, and animation frames. It also creates the necessary textures
 * for rendering the sprite using the provided renderer.
 *
 * @param renderer A pointer to the SDL_Renderer used to render the sprite.
 *                 It is used to create textures for the ring's animation frames.
 * @param frames A Frames structure containing the animation frames and related
 *               information for the ring sprite.
 * @return A Sprite structure representing the initialized ring with its
 *         properties and textures.
 */
Sprite initialize_ring(SDL_Renderer* renderer, Frames frames) {
    Sprite ring;
    ring.type = RING;
    ring.effects.effect_type = RING_EFFECT;
    ring.effects.ring_delta = RING_DELTA;
    ring.width = RING_WIDTH;
    ring.height = RING_HEIGHT;
    ring.x = WINDOW_WIDTH;
    ring.y = set_random_position(&ring);
    ring.speed = RING_SPEED;
    ring.current_frame = RING_CURRENT_FRAME;
    ring.collision_state = COLLISION_NONE;
    ring.animation_accumulator = 0;
    ring.frames = frames;
    create_texture(renderer, &ring);
    return ring;
}
