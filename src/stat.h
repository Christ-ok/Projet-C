#ifndef STAT_H
#define STAT_H
#include "animation.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void initImageDemon(SDL_Renderer *renderer);
void stat(SDL_Renderer *renderer, int demonCount);
void statCoeur(SDL_Renderer *renderer, Personnage *p);
void drawDemonStat(SDL_Renderer *renderer);
void drawCoeurStat(SDL_Renderer *renderer);
void cleanUpImageDemon();
void cleanUpImageHP();

#endif