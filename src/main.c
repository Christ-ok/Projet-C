#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "background.h"
#include "animation.h"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char *argv[])
{
    // Initialisation SDL et SDL_image
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        return -1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        SDL_Log("IMG_Init: %s", IMG_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("BLADE QUEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialisation modules
    initBackground(renderer);
    initCharacter(renderer);

    SDL_bool running = SDL_TRUE;
    int state = 0; // 0 = idle, 1 = run, 2 = attack

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_d)
                    state = 1;
                if (event.key.keysym.sym == SDLK_k)
                    state = 2;
            }
            if (event.type == SDL_KEYUP)
            {
                state = 0;
            }
        }

        SDL_RenderClear(renderer);
        drawBackground(renderer);
        drawCharacter(renderer, WIDTH / 2, HEIGHT / 2, state);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Nettoyage
    cleanupBackground();
    cleanupCharacter();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
