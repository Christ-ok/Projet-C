#include "background.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Texture *textureBackground = NULL;

void initBackground(SDL_Renderer *renderer){
    textureBackground = IMG_LoadTexture(renderer, "assets/background/PNG/City2/Bright/City2.png");
    if (!textureBackground)
        SDL_Log("Erreur chargement background: %s", IMG_GetError());
}

void drawBackground(SDL_Renderer *renderer, int x){
    if (!textureBackground){
        return;
    }

    SDL_Rect dst;
    dst.x = x;
    dst.y = 0;
    dst.w = 800;
    dst.h = 600;

    SDL_RenderCopy(renderer, textureBackground, NULL, &dst);
}

void cleanupBackground(){
    if (textureBackground)
        SDL_DestroyTexture(textureBackground);
}
