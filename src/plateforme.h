#ifndef PLATEFORME_H
#define PLATEFORME_H

#include <SDL2/SDL.h>
#include "animation.h"

#define TILE_SIZE 24

typedef struct
{
    int x, y;
    int x_base;
    int y_base;
    int h, w;
    int tiles_x;
    int tiles_y;
    int ground;
    int deplacement;
} Plateforme;

void spawn_Plateforme(Plateforme *Ptf, int x, int y, int tiles_x, int tiles_y);
void initPlateformeTexture(SDL_Renderer *renderer);
int verifyRectSuperposition(Personnage *p, Plateforme *Pft, int jump_offset);
int verifyRectSuperposition(Personnage *p, Plateforme *Pft, int jump_offset);
void checkCollisionWithPlateform(Plateforme *Ptf, Personnage *p, int jump_offset, int *jump_direction);
void Plateforme_draw(SDL_Renderer *renderer, Plateforme *Ptf, int camera_x);
void cleanUpPlateforme();

#endif