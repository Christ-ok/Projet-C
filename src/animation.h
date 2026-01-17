#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>

typedef struct
{
    int HP;
    int alive;
    int dead;
    int x;
    int y;
    float base_y;
    int w;
    int h;
    int speed;
    int state;
    int direction;
    int jump_direction;
    int jump_height;
    int jump_speed;
    int on_ground;
} Personnage;

void initCharacter(SDL_Renderer *renderer, Personnage *p);
void Hero_takeDamage(Personnage *p, int damage);
void drawCharacter(SDL_Renderer *renderer, Personnage *p, int camera_x, int camera_y, int jump_offset);
void updateCharacter(Personnage *p, int d_pressed, int q_pressed, int space_pressed, int camera_x);
void cleanupCharacter();

#endif
