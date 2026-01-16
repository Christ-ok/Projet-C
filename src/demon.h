#ifndef DEMON_H
#define DEMON_H

#include <SDL2/SDL.h>
#include "animation.h"


typedef struct
{
    int x, y;
    int y_base;
    int x_base;
    int HP;
    int dead;
    int exists;

    int currentState;
    int currentFrame;
    int locked;
    Uint32 lastFrameTime;

    //int statePosition
    int isLeft;
} Demon;


/*
    void (Demon *d, d_pressed){
        const int deplacer = d->statePosition; 15

        if (d_pressed){
            d->statePosition = deplacer; 15
        }

    }

*/

void initDemon(SDL_Renderer *renderer);
void Demon_spawn(Demon *d, int x, int y);
void drawDemon(SDL_Renderer *renderer, Demon *d, int state);
void updateDemon(Demon *d, int d_pressed, int q_pressed, int space_pressed, int *camera_x, int *camera_y, Personnage *p);
void Demon_takeDamage(Demon *d, int damage);
void cleanupDemon();

#endif