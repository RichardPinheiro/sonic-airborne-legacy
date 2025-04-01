#include "game.h"

/**
 * @brief Loads textures for each frame in the Frames structure.
 *
 * This function iterates over the frame paths in the Frames structure,
 * loads each image as an SDL_Surface, retrieves its dimensions, and
 * creates an SDL_Texture from it. The textures and their dimensions
 * are stored in the Frames structure.
 *
 * @param frames A pointer to a Frames structure containing paths and
 *        storage for textures and their dimensions.
 * @param renderer A pointer to the SDL_Renderer used to create textures.
 */
void load_texture(Frames* frames, SDL_Renderer* renderer) {
    for (size_t i = 0; i < frames->length; i++) {
        SDL_Surface* surface = IMG_Load(frames->paths[i]);
        if (!surface) {
            printf("Failed to load %s: %s\n", frames->paths[i], IMG_GetError());
            exit(EXIT_FAILURE);
        }
        frames->widths[i] = surface->w;
        frames->heights[i] = surface->h;
        SDL_FreeSurface(surface);
        frames->texture[i] = IMG_LoadTexture(renderer, frames->paths[i]);
        if (!frames->texture[i]) {
            printf("Texture creation failed: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * @brief Updates the sprite's animation frame based on elapsed time and updates dimensions.
 * 
 * This function manages frame progression using an accumulator pattern to track
 * elapsed time between frame changes. It advances the animation frame when the
 * accumulated time exceeds the frame delay duration.
 * It also updates dimensions when frame actually changes.
 * 
 * @param sprite Pointer to Sprite with animation properties
 * @param delta_time Milliseconds elapsed since last update
 */
void sprite_animation(Sprite *sprite, Uint32 delta_time) {
    sprite->animation_accumulator += delta_time;
    while (sprite->animation_accumulator >= sprite->frames.delay) {
        sprite->current_frame = (sprite->current_frame + 1) % sprite->frames.length;
        sprite->animation_accumulator -= sprite->frames.delay;
        sprite->width = sprite->frames.widths[sprite->current_frame];
        sprite->height = sprite->frames.heights[sprite->current_frame];
    }
}

/**
 * @brief Updates the position and boundaries of a sprite based on its speed and elapsed time.
 *
 * This function calculates the new position of the sprite by applying its speed
 * and a time scale factor derived from the elapsed time. If the sprite moves off
 * the left edge of the screen, it wraps around to the right edge and is assigned
 * a new random vertical position. The sprite's boundaries are updated accordingly.
 *
 * @param sprite Pointer to the Sprite whose position and boundaries are to be updated.
 * @param delta_time The time elapsed since the last update, in milliseconds.
 */
void sprite_motion(Sprite *sprite, Uint32 delta_time) {
    const float scaled_width = sprite->width * sprite->scale;
    sprite->x += sprite->speed * get_time_scale_factor(delta_time);
    if (sprite->x + (scaled_width / 2) < 0) {
        sprite->x = WINDOW_WIDTH + (scaled_width / 2);
        sprite->y = get_random_y_position(sprite);
    }
    update_sprite_boundaries(sprite);
}

/**
 * @brief  Renders the sprite with scaling and sub-pixel positioning.
 * 
 * This function draws the sprite using its current position, dimensions, and
 * animation frame texture. The rendering position is derived from the sprite's
 * floating-point coordinates converted to integer values for SDL rendering.
 * It also calculates scaled dimensions from base size (e.g., zoom in/out).
 * 
 * @param sprite Pointer to Sprite to render
 * @param renderer SDL_Renderer target for drawing operations
 */
void sprite_render(Sprite *sprite, SDL_Renderer* renderer) {
    const int scaled_width = (int)(sprite->width * sprite->scale);
    const int scaled_height = (int)(sprite->height * sprite->scale);
    SDL_Rect sprite_rect = {
        (int)roundf(sprite->x) - scaled_width / 2,
        (int)roundf(sprite->y) - scaled_height / 2,
        scaled_width,
        scaled_height
    };
    SDL_Texture* texture = sprite->frames.texture[sprite->current_frame];
    SDL_RenderCopy(renderer, texture, NULL, &sprite_rect);
}

/**
 * @brief Generates a random position keeping the sprite fully visible.
 * 
 * Accounts for centered positioning and scaled dimensions to ensure
 * the sprite never spawns partially off-screen.
 * 
 * @param sprite Pointer to Sprite (uses base_width/base_height and scale)
 * @return Y-coordinate in safe range [half_height, WINDOW_HEIGHT - half_height]
 */
int get_random_y_position(const Sprite* sprite) {
    const int half_height = (int)(sprite->height * sprite->scale) / 2;
    return half_height + rand() % (WINDOW_HEIGHT - 2 * half_height);
}

/**
 * @brief Calculates the vertical center position for a sprite.
 *
 * This function computes the vertical offset needed to center a sprite within the window.
 * It determines half of the sprite's scaled height (accounting for its scale factor) and
 * then positions it so that its center aligns with the vertical center of the window.
 *
 * @param sprite A pointer to the Sprite structure containing the sprite's height and scale factor.
 * @return The vertical position offset required to center the sprite in the window.
 */
float get_vertical_center_offset(const Sprite* sprite) {
    const float half_height = (sprite->height * sprite->scale) / 2.0f;
    return (WINDOW_HEIGHT - half_height) / 2.0f;
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
    return(
        sprite_a->boundary_left < sprite_b->boundary_right &&
        sprite_a->boundary_right > sprite_b->boundary_left &&
        sprite_a->boundary_top < sprite_b->boundary_bottom &&
        sprite_a->boundary_bottom > sprite_b->boundary_top
    );
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
    const float scaled_width = sprite->width * sprite->scale;
    const float scaled_height = sprite->height * sprite->scale;
    sprite->boundary_left = sprite->x - scaled_width / 2;
    sprite->boundary_right = sprite->x + scaled_width / 2;
    sprite->boundary_top = sprite->y - scaled_height / 2;
    sprite->boundary_bottom = sprite->y + scaled_height / 2;
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
    switch (sprite->effects.effect_type) {
        case DAMAGE_EFFECT: apply_penalties(sprite, sonic); break;
        case RING_EFFECT: apply_bonus(sprite, sonic); break;
        case LIFE_EFFECT: apply_Life(sprite, sonic); break;
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

void apply_penalties(Sprite* source, Sprite* target) {
    emit_life_change(source, target);
    emit_rings_change(source, target);
    emit_sfx(get_collision_sound(source->type));
}

void apply_bonus(Sprite *source, Sprite *target) {
    emit_rings_change(source, target);
    emit_sfx(get_collision_sound(source->type));
}

void apply_Life(Sprite* source, Sprite* target) {
    emit_life_change(source, target);
    emit_sfx(get_collision_sound(source->type));
}
