#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "background.h"
#include "animation.h"
#include "deplacement.h"
#include "deplacement2.h"



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

    initCharacter(renderer);
    initBackground(renderer);
    initBackground(renderer);

    SDL_bool running = SDL_TRUE;
    int gauche = 0;
    int state = 0; 
    int scroll = 0;
    int background_x = 0;
    int background_y = 0;

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
                    state = 1;
                    gauche = 0;                    
                }
                
                if(event.key.keysym.sym == SDLK_q)
                {
                    q_pressed = 1;
                    state = 1;
                    gauche = 1; 
                }

                if (event.key.keysym.sym == SDLK_k)
                {
                    state = 2;
                }

                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    space_pressed = 1;
                    state = 3;
                    jump_direction = 1;
                }

                if (event.key.keysym.sym == SDLK_t)
                {
                    state = 4;
                }

            }

            if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_d)
                {
                    d_pressed = 0;
                    state = 0;
                }

                if(event.key.keysym.sym == SDLK_k)
                {
                    state = 0;
                }

                if (event.key.keysym.sym == SDLK_q)
                {
                    state = 0;
                    q_pressed = 0;
                }

                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    state = 0;
                    space_pressed = 0;
                }

                if (event.key.keysym.sym == SDLK_t)
                {
                    state = 0;
                }
            }
        }

        if (d_pressed)
        {
            background_x -= speed;
        }
        
        if (q_pressed){
            background_x += speed;
        }

        if (background_x <= -800){
            background_x = 0;
        }

        if (background_x >= 0){
            background_x = 0;
        }
       

        if (space_pressed && jump_direction == 0)
        {
            jump_direction = 1;
        }

        if (jump_direction == 1)
        {
            jump_offset += jump_speed;
            if (jump_offset >= jump_height)
            {
                jump_offset = jump_height;
                jump_direction = -1;
            }
        } else if (jump_direction == -1)
        {
            jump_offset -= jump_speed;
            if (jump_offset <= 0)
            {
                jump_offset = 0;
                jump_direction = 0;
            }
        }    
        
        
        SDL_RenderClear(renderer);
        drawBackground(renderer, background_x, jump_offset);
        drawCharacter(renderer, WIDTH / 2, HEIGHT / 2, state, gauche);
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

