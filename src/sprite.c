#include "game.h"

/**
 * @brief Loads and initializes textures for a sprite's animation frames.
 * 
 * This function loads image files specified in the sprite's frame paths into 
 * GPU textures using the provided renderer. It initializes the sprite's texture 
 * array with 1:1 mapping to its frame paths.
 * 
 * @param renderer SDL_Renderer used for texture creation
 * @param sprite Pointer to Sprite struct containing frame path data
 */
void create_texture(SDL_Renderer* renderer, Sprite *sprite) {
    for (size_t i = 0; i < sprite->frames.length; i++) {
        sprite->frames.texture[i] = IMG_LoadTexture(renderer, sprite->frames.paths[i]);
        if (!sprite->frames.texture[i]) {
            printf("Failed to load texture '%s': %s\n", sprite->frames.paths[i], IMG_GetError());
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * @brief Updates the sprite's animation frame based on elapsed time.
 * 
 * This function manages frame progression using an accumulator pattern to track
 * elapsed time between frame changes. It advances the animation frame when the
 * accumulated time exceeds the frame delay duration.
 * 
 * @param sprite Pointer to Sprite with animation properties
 * @param delta_time Milliseconds elapsed since last update
 */
void sprite_animation(Sprite *sprite, Uint32 delta_time) {
    sprite->animation_accumulator += delta_time;
    if (sprite->animation_accumulator >= sprite->frames.delay) {
        sprite->current_frame = (sprite->current_frame + 1) % sprite->frames.length;
        sprite->animation_accumulator -= sprite->frames.delay;
    }
}

void sprite_motion(Sprite *sprite, Uint32 delta_time) {
    sprite->x += sprite->speed * get_time_scale_factor(delta_time);
    if (sprite->x + sprite->width < 0) {
        sprite->x = WINDOW_WIDTH + sprite->width;
        sprite->y = set_random_position(sprite);
    }
    update_sprite_boundaries(sprite);
}

/**
 * @brief Renders the sprite's current animation frame to the specified renderer.
 * 
 * This function draws the sprite using its current position, dimensions, and
 * animation frame texture. The rendering position is derived from the sprite's
 * floating-point coordinates converted to integer values for SDL rendering.
 * 
 * @param sprite Pointer to Sprite to render
 * @param renderer SDL_Renderer target for drawing operations
 */
void sprite_render(Sprite *sprite, SDL_Renderer* renderer) {
    SDL_Rect sprite_rect = {
        (int)sprite->x,
        (int)sprite->y,
        sprite->width,
        sprite->height
    };
    SDL_RenderCopy(renderer, sprite->frames.texture[sprite->current_frame], NULL, &sprite_rect);
}

/**
 * @brief Generates a random vertical position within window boundaries.
 * 
 * This function calculates a safe vertical position that keeps the sprite fully
 * visible within the application window's height constraints.
 * 
 * @param sprite Pointer to Sprite used for dimension calculations
 * @return Integer Y-coordinate in range [0, WINDOW_HEIGHT - sprite height]
 */
int set_random_position(Sprite *sprite) {
    return rand() % (WINDOW_HEIGHT - sprite->height);
}

/**
 * @brief Frees the memory allocated for the textures of a sprite's frames.
 * 
 * This function iterates through the textures of a sprite's frames and 
 * destroys each texture using SDL_DestroyTexture. It then frees the memory 
 * allocated for the texture array and sets the pointer to NULL.
 * 
 * @param sprite Pointer to the sprite whose frame textures need to be freed.
 */
void free_sprite_frames(Sprite *sprite) {
    if (sprite->frames.texture) {
        for (size_t i = 0; i < sprite->frames.length; i++)
            if (sprite->frames.texture[i]) SDL_DestroyTexture(sprite->frames.texture[i]);
        free(sprite->frames.texture);
        sprite->frames.texture = NULL;
    }
}

/**
 * @brief Calculates the time scale factor based on the given delta time.
 * 
 * This function computes a scaling factor for time-related calculations,
 * normalizing the delta time against a predefined normalization factor.
 * 
 * @param delta_time The time elapsed since the last frame, in milliseconds.
 * @return The time scale factor as a float, representing the ratio of the 
 *         delta time to the normalization factor. (delta_time / 1000.0f)
 */
float get_time_scale_factor(Uint32 delta_time) {
    return (float)delta_time / NORMALIZATION_FACTOR;
}

/**
 * @brief hecks for collision between two sprites using Axis-Aligned Bounding Boxes (AABB).
 * 
 * This function determines if two sprites overlap by comparing their bounding boxes.
 * AABB collision works by checking if the rectangles of the two sprites intersect on both
 * the X and Y axes.
 * 
 * @param sprite_a First sprite (e.g., player, enemy, or coin).
 * @param sprite_b Second sprite (e.g., player, enemy, or coin).
 * @return true` if the sprites' bounding boxes overlap, `false` otherwise.
 */
bool check_collision(Sprite *sprite_a, Sprite *sprite_b) {
    bool x_overlap = sprite_a->boundary_left < sprite_b->boundary_right &&
        sprite_a->boundary_right > sprite_b->boundary_left;
    bool y_overlap = sprite_a->boundary_top < sprite_b->boundary_bottom &&
        sprite_a->boundary_bottom > sprite_b->boundary_top;
    return x_overlap && y_overlap;
}

/**
 * @brief Updates the precomputed boundaries of the given sprite.
 * 
 * This function calculates and updates the left, right, top, and bottom boundaries
 * of the sprite based on its current position and dimensions. These boundaries are
 * used for collision detection.
 * 
 * @param sprite Pointer to the sprite whose boundaries need to be updated.
 */
void update_sprite_boundaries(Sprite *sprite) {
    sprite->boundary_left = sprite->x;
    sprite->boundary_right = sprite->x + sprite->width;
    sprite->boundary_top = sprite->y;
    sprite->boundary_bottom = sprite->y + sprite->height;
}

/**
 * @brief Updates the collision states of the given sprites with respect to the player (Sonic).
 * 
 * This function iterates through the array of sprites and updates their collision states
 * based on their current positions and the player's position. The collision states are used
 * to determine the behavior of the game logic when collisions occur.
 * 
 * @param sonic Pointer to the player's sprite.
 * @param sprites Array of pointers to the sprites to be checked for collisions.
 * @param sprites_length Length of the sprites array.
 */
void update_collision_states(Sprite *sonic, Sprite **sprites, size_t sprites_length) {
    for (size_t i = 0; i < sprites_length; i++) {
        bool is_colliding = check_collision(sonic, sprites[i]);
        Sprite *sprite = sprites[i];
        switch (sprite->collision_state) {
            case COLLISION_NONE:
                if (is_colliding) sprite->collision_state = COLLISION_ENTER;
                break;
            case COLLISION_ENTER:
            case COLLISION_STAY:
                sprite->collision_state = is_colliding ? COLLISION_STAY : COLLISION_EXIT;
                break;
            case COLLISION_EXIT:
                sprite->collision_state = is_colliding ? COLLISION_ENTER : COLLISION_NONE;
                break;
        }
    }
}

/**
 * @brief Handles collision events between the player (Sonic) and other sprites.
 * 
 * This function processes the collision states of sprites with respect to the player,
 * executing appropriate actions based on the type of collision event (enter, stay, exit).
 * 
 * @param sonic Pointer to the player's sprite.
 * @param sprites Array of pointers to the sprites to be checked for collisions.
 * @param sprites_length Length of the sprites array.
 */
void handle_collisions(Sprite *sonic, Sprite **sprites, size_t sprites_length) {
    for (size_t i = 0; i < sprites_length; i++) {
        Sprite *sprite = sprites[i];
        switch (sprite->collision_state) {
            case COLLISION_ENTER: handle_collision_enter(sprite, sonic); break;
            case COLLISION_STAY: handle_collision_stay(sprite, sonic); break;
            case COLLISION_EXIT: handle_collision_exit(sprite, sonic); break;
        }
    }
}

void handle_collision_enter(Sprite *sprite, Sprite *sonic) {
    switch (sprite->effect_type) {
        case EFFECT_DAMAGE: apply_damage_to_player(sprite, sonic); break;
        case EFFECT_RINGS: apply_rings_to_player(sprite, sonic); break;
    }
}

void handle_collision_stay(Sprite *sprite, Sprite *sonic) {
    // printf("Optional: Ongoing effects (e.g., damage over time) %d:%d\n",
    //     sprite->collision_state, sonic->collision_state);
}

void handle_collision_exit(Sprite *sprite, Sprite *sonic) {
    sprite->collision_state = COLLISION_NONE;
    sonic->collision_state = COLLISION_NONE;
}
