#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "background.h"
#include "animation.h"
#include "deplacement.h"
#include "deplacement2.h"

Personnage perso;

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char *argv[])
{
    
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

    initCharacter(renderer, &perso);
    initBackground(renderer);

    SDL_bool running = SDL_TRUE; 
    int camera_x = 0;
    int camera_y = 0;

    int d_pressed = 0;
    int q_pressed = 0;
    int space_pressed = 0;

    int jump_offset = 0;
    int jump_direction = 0;
    const int jump_height = 120;
    const int jump_speed = 4;

    const int speed = 5;

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
                {
                    d_pressed = 1;
                    perso.state = 1;
                    perso.direction = 0;                 
                }
                
                if(event.key.keysym.sym == SDLK_q)
                {
                    q_pressed = 1;
                    perso.state = 1;
                    perso.direction = 1; 
                }

                if (event.key.keysym.sym == SDLK_k)
                {
                    perso.state = 2;
                }

                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    space_pressed = 1;
                    perso.state = 3;
                    jump_direction = 1;
                }

                if (event.key.keysym.sym == SDLK_t)
                {
                    perso.state = 4;
                }
            }

            if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_d)
                {
                    d_pressed = 0;
                    perso.state = 0;
                }

                if(event.key.keysym.sym == SDLK_k)
                {
                    perso.state = 0;
                }

                if (event.key.keysym.sym == SDLK_q)
                {
                    perso.state = 0;
                    q_pressed = 0;
                }

                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    perso.state = 0;
                    space_pressed = 0;
                }

                if (event.key.keysym.sym == SDLK_t)
                {
                    perso.state = 0;
                }
            }
        }

        updateCharacter(&perso, d_pressed, q_pressed, space_pressed, camera_x);

        jumpY(space_pressed, &jump_direction, &jump_offset, jump_height, jump_speed);
        if (jump_direction == 0 && jump_offset == 0 && perso.state == 3)
        {
            perso.state = 0;
        }

        cameraY(&camera_x, &camera_y, &perso, jump_offset, WIDTH);
        
        SDL_RenderClear(renderer);
        drawBackground(renderer, camera_x, camera_y);
        drawCharacter(renderer, &perso, camera_x, camera_y, jump_offset);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    cleanupBackground();
    cleanupCharacter();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

