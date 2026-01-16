#ifndef DEPLACEMENT2_H
#define DEPLACEMENT2_H
#include <SDL2/SDL.h>
#include "animation.h"

void jumpY(int space_pressed, int *jump_direction, int *jump_offset, int jump_height, int jump_speed, int on_ground);
void cameraY(int *camera_x, int *camera_y, Personnage *p, int jump_offset, int screen_width);

#endif