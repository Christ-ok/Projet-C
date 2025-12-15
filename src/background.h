#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>

void initBackground(SDL_Renderer *renderer); 
void drawBackground(SDL_Renderer *renderer, int x);
void cleanupBackground();

#endif
