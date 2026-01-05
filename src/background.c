#include "background.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Texture *textureBackground = NULL;

void initBackground(SDL_Renderer *renderer){
    textureBackground = IMG_LoadTexture(renderer, "assets/background/PNG/City2/Bright/City2.png");
    if (!textureBackground)
        SDL_Log("Erreur chargement background: %s", IMG_GetError());
}

SDL_Texture *getBackgroundTExture(SDL_Renderer *renderer){
    return textureBackground;
}


void drawBackground(SDL_Renderer *renderer, int x, int y){
    if (!textureBackground){
        return;
    }

    SDL_Rect dst1 = {x, y, 800, 600};
    SDL_Rect dst2 = {x + 800, y, 800, 600};

    SDL_RenderCopy(renderer, textureBackground, NULL, &dst1);
    SDL_RenderCopy(renderer, textureBackground, NULL, &dst2);
}


void cleanupBackground(){
    if (textureBackground)
        SDL_DestroyTexture(textureBackground);
}
