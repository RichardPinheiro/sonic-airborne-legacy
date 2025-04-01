#include "game.h"

/**
 * @brief Creates a new Sonic sprite with the given renderer.
 *
 * This function initializes the Sonic sprite by loading its animation frames,
 * setting its initial position, dimensions, speed, and other properties.
 * It then calls the initialize_sonic function to further configure the sprite.
 *
 * @param renderer The SDL_Renderer used to create the sprite's textures.
 * @return A new Sonic sprite with the specified properties.
 */
Sprite create_sonic(SDL_Renderer* renderer) {
    const char* frame_paths[] = {
        "assets/sprites/sonic/sonic_1.png",
        "assets/sprites/sonic/sonic_2.png",
        "assets/sprites/sonic/sonic_3.png",
        "assets/sprites/sonic/sonic_4.png"
    };
    size_t frames_length = sizeof(frame_paths) / sizeof(frame_paths[0]);
    Frames frames = {
        frame_paths,
        frames_length,
        SONIC_FRAME_DELAY,
        malloc(sizeof(SDL_Texture*) * frames_length),
        malloc(sizeof(int) * frames_length),
        malloc(sizeof(int) * frames_length)
    };
    if (!frames.texture || !frames.widths || !frames.heights) {
        fprintf(stderr, "Failed to allocate memory for Frames resources.\n");
        exit(EXIT_FAILURE);
    }
    load_texture(&frames, renderer);
    return initialize_sonic(renderer, frames);
}

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
static void load_texture(Frames* frames, SDL_Renderer* renderer) {
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
 * @brief Initializes a Sonic sprite with specified frames and renderer.
 *
 * This function sets up the initial properties of a Sonic sprite, including
 * its position, dimensions, speed, and animation frames. It also creates
 * the texture for the sprite using the provided renderer.
 *
 * @param renderer The SDL_Renderer used to create the sprite's textures.
 * @param frames The Frames structure containing animation frame paths and details.
 * @return A Sprite structure representing the initialized Sonic sprite.
 */
Sprite initialize_sonic(SDL_Renderer* renderer, Frames frames) {
    Sprite sonic;
    sonic.type = PLAYER;
    sonic.life = SONIC_LIFE;
    sonic.rings = SONIC_RINGS;
    sonic.scale = SONIC_ZOOM_SCALE;
    sonic.width = frames.widths[0];
    sonic.height = frames.heights[0];
    sonic.x = 0;
    sonic.y = (WINDOW_HEIGHT - sonic.height) / 2;
    sonic.speed = SONIC_SPEED;
    sonic.current_frame = SONIC_CURRENT_FRAME;
    sonic.collision_state = COLLISION_NONE;
    sonic.hover_amplitude = 1.5f;
    sonic.hover_frequency = 0.006f;
    sonic.hover_start_time = SDL_GetTicks();
    sonic.velocity_x = 0;
    sonic.velocity_y = 0;
    sonic.acceleration = 0.4f;
    sonic.friction = 0.95f;
    sonic.animation_accumulator = 0;
    sonic.frames = frames;
    create_texture(renderer, &sonic);
    return sonic;
}

/**
 * @brief Updates the motion of the Sonic sprite based on player input and time.
 *
 * This function uses the time scale factor to update the Sonic sprite's
 * velocity and position by considering player interactions and applying friction.
 *
 * @param sonic A pointer to the Sprite structure representing Sonic.
 * @param delta_time The time elapsed since the last frame, used to scale motion.
 * @param keystates The SDL keyboard state array. Used to detect arrow key presses.
 */
void sonic_motion(Sprite *sonic, Uint32 delta_time, const Uint8 *keystates) {
    float time_scale_factor = get_time_scale_factor(delta_time);
    watch_player_interactions(sonic, keystates);
    apply_friction(sonic, time_scale_factor);
    update_position(sonic, time_scale_factor);
    check_boundary(sonic);
    update_sprite_boundaries(sonic);
}

/**
 * @brief Processes player input and updates Sonic's velocity or hover effect.
 *
 * This function handles keyboard input from the player and modifies Sonic's velocity
 * based on the arrow keys pressed. If no arrow keys are pressed, it triggers a hover
 * effect using a sine wave oscillation to simulate idle floating.
 *
 * @param sonic A Pointer to the Sprite structure representing Sonic.
 * @param keystates The SDL keyboard state array. Used to detect arrow key presses.
 */
void watch_player_interactions(Sprite *sonic, const Uint8 *keystates) {
    if (keystates[SDL_SCANCODE_LEFT]) sonic->velocity_x -= sonic->acceleration;
    if (keystates[SDL_SCANCODE_RIGHT]) sonic->velocity_x += sonic->acceleration;
    if (keystates[SDL_SCANCODE_UP]) sonic->velocity_y -= sonic->acceleration;
    if (keystates[SDL_SCANCODE_DOWN]) sonic->velocity_y += sonic->acceleration;
    bool arrow_pressed = is_arrow_pressed(keystates);
    if (arrow_pressed) sonic->hover_start_time = SDL_GetTicks();
    if (!arrow_pressed) apply_hover_effect(sonic);
}

/**
 * @brief Applies a hover effect to Sonic.
 *
 * This creates a smooth vertical oscillation using a sine wave, centered around Sonic's
 * starting Y position. The oscillation speed and amplitude are controlled by
 * `hover_frequency` and `hover_amplitude`.
 *
 * @param sonic Pointer to the Sprite structure representing Sonic.
 */
void apply_hover_effect(Sprite *sonic) {
    Uint32 current_time = SDL_GetTicks();
    Uint32 elapsed_time = current_time - sonic->hover_start_time;
    float oscillation = sinf(elapsed_time * sonic->hover_frequency) * sonic->hover_amplitude;
    sonic->y += oscillation;
}

/**
 * @brief Checks if any arrow keys (Left/Right/Up/Down) are currently pressed.
 *
 * This utility function scans the keyboard state array for the four arrow keys
 * and returns `true` if any of them are pressed.
 *
 * @param keystates The SDL keyboard state array.
 * @return `true` if any arrow key is pressed, `false` otherwise.
 */
bool is_arrow_pressed(const Uint8 *keystates) {
    return keystates[SDL_SCANCODE_LEFT] || 
        keystates[SDL_SCANCODE_RIGHT] ||
        keystates[SDL_SCANCODE_UP] ||
        keystates[SDL_SCANCODE_DOWN];
}

/**
 * @brief Applies friction to Sonic's velocity to simulate gradual slowdown.
 *
 * This function decays Sonic's X/Y velocity using an exponential friction model.
 * The friction factor is scaled by `time_scale_factor` to ensure consistent decay
 * across different frame rates.
 * It uses `pow(friction, time_scale_factor)` to apply frame-rate-independent decay.
 *
 * @param sonic Pointer to the Sprite structure representing Sonic.
 * @param time_scale_factor Frame-rate scaling factor.
 */
void apply_friction(Sprite *sonic, float time_scale_factor) {
    float friction_factor = powf(sonic->friction, time_scale_factor);
    sonic->velocity_x *= friction_factor;
    sonic->velocity_y *= friction_factor;
}

/**
 * @brief Updates Sonic's position based on his current velocity and time scaling.
 *
 * This function calculates Sonic's new position by scaling his X/Y velocity by
 * `time_scale_factor`, ensuring movement remains consistent across frame rates.
 *
 * @param sonic Pointer to the Sprite structure representing Sonic.
 * @param time_scale_factor Frame-rate scaling factor.
 */
void update_position(Sprite *sonic, float time_scale_factor) {
    sonic->x += sonic->velocity_x * time_scale_factor;
    sonic->y += sonic->velocity_y * time_scale_factor;
}

/**
 * @brief Clamps the sprite within window boundaries considering scaled size and centered position.
 *
 * This function ensures the sprite's *center* stays within the visible area while
 * accounting for its scaled dimensions. Uses half-width/height to prevent edges
 * from going out of bounds. It also calculates scaled half-dimensions to apply on
 * X-axis: Keep center between [half_width, WINDOW_WIDTH - half_width],
 * Y-axis: Keep center between [half_height, WINDOW_HEIGHT - half_height].
 *
 * @param sprite Sprite to clamp (modifies x/y)
 */
void check_boundary(Sprite *sprite) {
    const float half_width = (sprite->width * sprite->scale) / 2;
    const float half_height = (sprite->height * sprite->scale) / 2;
    sprite->x = fmaxf(half_width, fminf(sprite->x, WINDOW_WIDTH - half_width));
    sprite->y = fmaxf(half_height, fminf(sprite->y, WINDOW_HEIGHT - half_height));
}
