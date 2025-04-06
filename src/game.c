#include "game.h"

EventQueue global_queue;
GameOverState game_over_state;

int main(void) {
    initialize_event_queue();
    srand((unsigned int)time(NULL)); // Seed the random generator 

    // Initialize SDL with video and image support
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Initialize SDL_image for PNG support
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image initialization failed: %s\n", IMG_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Initialize SDL_mixer for sound support
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer initialization failed: %s\n", Mix_GetError());
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    char window_title[128];
    snprintf(window_title, sizeof(window_title), "%s - v%s", GAME_TITLE, GAME_VERSION);
    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        window_title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0 // SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");  // Linear filtering
    // Create accelerated vsync'd renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Load background image
    SDL_Texture* background = IMG_LoadTexture(renderer, "assets/backgrounds/stage3_bg.png");
    if (!background) {
        printf("Background loading failed: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    Sprite sonic = create_sonic(renderer);
    // Sprite ring = create_ring(renderer);
    // Sprite life = create_life(renderer);
    Sprite buzz = create_buzz_enemy(renderer);
    Sprite game_over = create_game_over(renderer);
    Sprite *sprites[] = {
        // &ring,
        // &life,
        &buzz
    };
    size_t sprites_length = sizeof(sprites) / sizeof(sprites[0]);

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

        sprite_animation(&sonic, delta_time);
        // sprite_animation(&ring, delta_time);
        // sprite_animation(&life, delta_time);
        sprite_animation(&buzz, delta_time);

        sonic_motion(&sonic, delta_time);
        // sprite_motion(&ring, delta_time);
        // sprite_motion(&life, delta_time);
        sprite_motion(&buzz, delta_time);
        game_over_motion(&game_over, delta_time);

        update_collision_states(&sonic, sprites, sprites_length);
        handle_collisions(&sonic, sprites, sprites_length);

        event_listener(&global_queue);

        SDL_RenderClear(renderer); // Clear the screen
        SDL_RenderCopy(renderer, background, NULL, NULL); // Render the background

        sprite_render(&sonic, renderer);
        // sprite_render(&ring, renderer);
        // sprite_render(&life, renderer);
        sprite_render(&buzz, renderer);
        sprite_render(&game_over, renderer);

        SDL_RenderPresent(renderer); // Update the display
        last_frame_time = current_time; // Update timing for next frame
    }

    // Clean up
    free_sprite_frames(&sonic);
    // free_sprite_frames(&ring);
    // free_sprite_frames(&life);
    free_sprite_frames(&buzz);
    free_sprite_frames(&game_over);
    SDL_DestroyTexture(background);
    audio_cleanup();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
