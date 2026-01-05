#ifndef DEMON_H
#define DEMON_H

#include <SDL2/SDL.h>

void initDemon(SDL_Renderer *renderer);
void drawDemon(SDL_Renderer *renderer, int x, int y, int state, int PlayerIsLeft);
void Demon_takeDamage(int damage);
void cleanupDemon();

#endif