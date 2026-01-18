#ifndef PAUSE_H
#define PAUSE_H

#include <SDL2/SDL.h>

#include "animation.h"
#include "demon.h"

typedef struct {
    SDL_Rect save_btn;
    SDL_Rect quit_btn;
} PauseButtons;

void togglePause(SDL_Renderer *renderer, int width, int height, int camera_x, int camera_y, int jump_offset, int state, int gauche, int PlayerIsLeft, Personnage *perso, Demon *horde);
int isGamePaused();
void renderPauseMenu(SDL_Renderer *renderer, int width, int height);
int handlePauseButtons(int mouseX, int mouseY, PauseButtons *buttons);
void cleanupPause();

#endif
