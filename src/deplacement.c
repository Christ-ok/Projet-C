#include "deplacement.h"
#include "background.h"
#include <SDL2/SDL_image.h>

void deplacerJoueur(int *background_x, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_d)
        {
            *background_x -= 5;
        }
    }

}
