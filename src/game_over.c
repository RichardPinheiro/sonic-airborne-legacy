#include "game.h"

/**
 * @brief Creates a Game Over sprite with specified frames.
 *
 * This function initializes a Game Over sprite by loading the necessary frames,
 * setting up the animation, and allocating memory for the frames' resources.
 *
 * @param renderer The SDL_Renderer used to create the textures for the frames.
 * @return A Sprite structure representing the initialized Game Over.
 */
Sprite create_game_over(SDL_Renderer* renderer) {
    const char* frame_paths[] = { "assets/images/game_over.png" };
    size_t frames_length = sizeof(frame_paths) / sizeof(frame_paths[0]);
    Frames frames = {
        frame_paths,
        frames_length,
        GAME_OVER_FRAME_DELAY,
        malloc(sizeof(SDL_Texture*) * frames_length),
        malloc(sizeof(int) * frames_length),
        malloc(sizeof(int) * frames_length)
    };
    if (!frames.texture || !frames.widths || !frames.heights) {
        fprintf(stderr, "Failed to allocate memory for Frames resources.\n");
        exit(EXIT_FAILURE);
    }
    load_texture(&frames, renderer);
    printf("After load_texture: %d\n", frames.widths[0]);
    return initialize_game_over(frames);
}

/**
 * @brief Initializes a Game Over sprite with the given frames.
 *
 * This function takes a Frames structure as input and initializes a Sprite
 * representing the Game Over. It sets the sprite's type, scale, current frame,
 * dimensions, position, target position, and speed, as well as assigns the given
 * frames to the sprite.
 *
 * @param frames The Frames structure containing the textures, widths, and heights
 *               of the Game Over's frames.
 * @return A Sprite structure representing the initialized Game Over.
 */
Sprite initialize_game_over(Frames frames) {
    Sprite game_over;
    game_over.type = GAME_OVER;
    game_over.scale = GAME_OVER_ZOOM_SCALE;
    game_over.current_frame = GAME_OVER_CURRENT_FRAME;
    game_over.width = frames.widths[game_over.current_frame];
    game_over.height = frames.heights[game_over.current_frame];
    game_over.x = GAME_OVER_INITIAL_X;
    game_over.y = GAME_OVER_INITIAL_Y;
    game_over.target_y = GAME_OVER_TARGET_Y;
    game_over.speed = GAME_OVER_SPEED;
    game_over.frames = frames;
    return game_over;
}

/**
 * @brief Updates the position of the Game Over sprite based on time.
 *
 * This function moves the Game Over sprite vertically by incrementing its y-coordinate
 * based on the elapsed time and the sprite's speed. If the sprite reaches its target
 * y-coordinate, it stops moving.
 *
 * @param game_over A pointer to the Game Over sprite to be updated.
 * @param delta_time The time elapsed since the last frame, in milliseconds.
 *
 * @return void
 */
void game_over_motion(Sprite *game_over, Uint32 delta_time) {
    if (!game_over_state.is_active) return;
    game_over->y += game_over->speed * get_time_scale_factor(delta_time);
    if (game_over->y < game_over->target_y) game_over->y = game_over->target_y;
}
