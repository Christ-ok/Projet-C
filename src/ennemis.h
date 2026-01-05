#ifndef ENNEMIS_H
#define ENNEMIS_H

#include <SDL2/SDL.h>

void initEnnemy(SDL_Renderer *renderer);
void drawEnnemy(SDL_Renderer *renderer, int x, int y, int state);
void cleanupEnnemy();

#endif