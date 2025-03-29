#include "game.h"

EventQueue global_queue;

int main(void) {
    initialize_event_queue();
    srand(time(NULL)); // Seed the random generator 

    // Initialize SDL with video and image support
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_image for PNG support
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image initialization failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialize SDL_mixer for sound support
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer initialization failed: %s\n", Mix_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Sonic in the Air",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0
    );
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Load background image
    SDL_Texture* background = IMG_LoadTexture(renderer, "assets/backgrounds/stage2_bg.png");
    if (!background) {
        printf("Background loading failed: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    Sprite sonic = create_sonic(renderer);
    Sprite buzz = create_buzz_enemy(renderer);
    Sprite *sprites[] = {
        &buzz
    };
    int sprites_length = sizeof(sprites) / sizeof(sprites[0]);

    audio_initialization();

    // Game loop variables
    bool quit = false;
    SDL_Event event;

    // Main game loop
    while (!quit) {
        // Handle events (e.g., window close)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true; // Exit when the window is closed
            }
        }

        static Uint32 last_frame_time = 0;
        Uint32 current_time = SDL_GetTicks();
        Uint32 delta_time = current_time - last_frame_time;

        // Cap to ~60 FPS (16ms per frame)
        if (delta_time < TARGET_FRAME_TIME) {
            SDL_Delay(TARGET_FRAME_TIME - delta_time);
            current_time = SDL_GetTicks();
            delta_time = current_time - last_frame_time;
        }

        const Uint8 *keystates = SDL_GetKeyboardState(NULL);
        sonic_motion(&sonic, delta_time, keystates);
        enemy_motion(&buzz, delta_time);

        sprite_animation(&buzz, delta_time);
        sprite_animation(&sonic, delta_time);

        update_collision_states(&sonic, sprites, sprites_length);
        handle_collisions(&sonic, sprites, sprites_length);

        process_events(&global_queue);

        SDL_RenderClear(renderer); // Clear the screen
        SDL_RenderCopy(renderer, background, NULL, NULL); // Render the background

        sprite_render(&buzz, renderer);
        sprite_render(&sonic, renderer);

        SDL_RenderPresent(renderer); // Update the display
        last_frame_time = current_time; // Update timing for next frame
    }

    // Clean up
    free_sprite_frames(&buzz);
    free_sprite_frames(&sonic);
    SDL_DestroyTexture(background);
    audio_cleanup();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
