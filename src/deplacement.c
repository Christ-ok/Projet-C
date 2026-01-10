#include "deplacement.h"
#include "background.h"
#include <SDL2/SDL_image.h>


const int frameWidth2 = 96;
const int frameHeight2 = 84;
const int speed = 15;


void deplacerJoueur(int *background_x, int d_pressed, int q_pressed)
{
    if (d_pressed)
    {
        *background_x -= speed;
    }
        
    if (q_pressed){
        *background_x += speed;
    }
    
    if (*background_x <= -800){
        *background_x = 0;
    }
 
    if (*background_x >= 0){
        *background_x = 0;
    }
    


}
