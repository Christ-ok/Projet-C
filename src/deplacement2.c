#include "deplacement2.h"
#include "background.h"
#include <SDL2/SDL_image.h>

#define WIDTH 800
#define HEIGHT 600
#define SPEED 5

void deplacerBackgroundX(SDL_Texture *background, int *background_x, int *background_y, SDL_Event event){
    int backgroundWidth, backgroundHeight;

    SDL_QueryTexture(background, NULL, NULL, &backgroundWidth ,&backgroundHeight);


    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_q)
        {
            *background_x += 5;  

            if (*background_x >= WIDTH)
            {
                *background_x = 0;
            }            
        }

        if (event.key.keysym.sym == SDLK_SPACE)
        {
            *background_y -= SPEED;

            if (*background_y <= -HEIGHT / 6)
            {
                *background_y = 0;
            }    
        }
    }
}

