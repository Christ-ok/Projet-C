#ifndef PAUSE_H
#define PAUSE_H

#include <SDL2/SDL.h>

void togglePause(SDL_Renderer *renderer, int width, int height, int camera_x, int camera_y, int jump_offset, int state, int gauche, int PlayerIsLeft);
int isGamePaused();
void renderPauseMenu(SDL_Renderer *renderer, int width, int height);
void cleanupPause();

#endif
