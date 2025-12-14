#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>

void initCharacter(SDL_Renderer *renderer);
void drawCharacter(SDL_Renderer *renderer, int x, int y, int state, int gauche);
void cleanupCharacter();

#endif
