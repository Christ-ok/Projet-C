#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void initBackground(SDL_Renderer *renderer); 
SDL_Texture* getBackgroundTExture(SDL_Renderer *renderer);
void drawBackground(SDL_Renderer *renderer, int camera_x, int camera_y);
void cleanupBackground();


#endif
