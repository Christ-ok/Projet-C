#ifndef BLOC_H
#define BLOC_H

#include <SDL2/SDL.h>
#include "animation.h"

typedef struct{
    int x;
    int y;
    int w;
    int h;
    int ground;
    int transparant;
}Bloc;

void initBloc(SDL_Renderer *renderer, Bloc *b);
void initBlocArray(Bloc *tab, int nb_blocs, int blocWidth, int blocHeight);
void generateLevel(Bloc *level, int total_blocs, int blocWidth, int blocHeight);
void drawBloc(SDL_Renderer * renderer, Bloc *tab, int nb_blocs, int camera_x);
void applyGravity(Personnage *p);
void checkCollisionWithBlocs(Bloc *level, int nb_blocs, Personnage *p, int jump_offset, int *jump_direction);
void cleanupBloc();

#endif
