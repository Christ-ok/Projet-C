#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void GameOver(SDL_Renderer *renderer, int width, int height);
int handleGameOverButtons(int mouseX, int mouseY, int width, int height);
void cleanupGameOver();

#endif