#include "deplacement2.h"
#include "background.h"
#include "animation.h"
#include <SDL2/SDL_image.h>

#define WIDTH 800
#define HEIGHT 600
#define SPEED 5

void jumpY(int space_pressed, int *jump_direction, int *jump_offset, int jump_height, int jump_speed){
   
    if (space_pressed && *jump_direction == 0) *jump_direction = 1;
    
    if (*jump_direction == 1)
    {
        *jump_offset += jump_speed;
        if (*jump_offset >= jump_height)
        {
            *jump_offset = jump_height;
            *jump_direction = -1;
        }


    } else if (*jump_direction == -1){
        *jump_offset -= jump_speed;
        if (*jump_offset <= 0)
        {
            *jump_offset = 0;
            *jump_direction = 0;
        }
    }
}

void cameraY(int *camera_x, int *camera_y, Personnage *p, int jump_offset, int screen_width){

    *camera_x = p->x - WIDTH / 2;
    if (*camera_x < 0) *camera_x = 0;
    
    *camera_y = (jump_offset / 2);

}

