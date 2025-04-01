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
    const char* frame_paths[] = {
        "assets/sprites/ring/ring_1.png",
        "assets/sprites/ring/ring_2.png",
        "assets/sprites/ring/ring_3.png",
        "assets/sprites/ring/ring_4.png"
    };
    size_t frames_length = sizeof(frame_paths) / sizeof(frame_paths[0]);
    Frames frames = {
        frame_paths,
        frames_length,
        RING_FRAME_DELAY,
        malloc(sizeof(SDL_Texture*) * frames_length),
        malloc(sizeof(int) * frames_length),
        malloc(sizeof(int) * frames_length)
    };
    if (!frames.texture || !frames.widths || !frames.heights) {
        fprintf(stderr, "Failed to allocate memory for Frames resources.\n");
        exit(EXIT_FAILURE);
    }
    load_texture(&frames, renderer);
    return initialize_ring(frames);
}

/**
 * @brief Initializes a ring sprite with specified animation frames and properties.
 *
 * This function sets up a ring sprite by assigning its type, effect, dimensions,
 * position, speed, and animation frames.
 *
 * @param frames A Frames structure containing the animation frames and related
 *               information for the ring sprite.
 * @return A Sprite structure representing the initialized ring with its
 *         properties and textures.
 */
Sprite initialize_ring(Frames frames) {
    Sprite ring;
    ring.type = RING;
    ring.effects.effect_type = RING_EFFECT;
    ring.effects.ring_delta = RING_DELTA;
    ring.scale = RING_ZOOM_SCALE;
    ring.width = frames.widths[0];
    ring.height = frames.heights[0];
    ring.x = WINDOW_WIDTH;
    ring.y = set_random_y_position(&ring);
    ring.speed = RING_SPEED;
    ring.current_frame = RING_CURRENT_FRAME;
    ring.collision_state = COLLISION_NONE;
    ring.animation_accumulator = 0;
    ring.frames = frames;
    return ring;
}
