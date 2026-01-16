#include "background.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Texture *textureBackground = NULL;
int backgroundWidth = 0;
int backgroundHeight = 0;

void initBackground(SDL_Renderer *renderer){
    textureBackground = IMG_LoadTexture(renderer, "assets/background/PNG/City2/Bright/City2.png");
    if (!textureBackground) SDL_Log("Erreur chargement background: %s", IMG_GetError());

    SDL_QueryTexture(textureBackground, NULL, NULL, &backgroundWidth ,&backgroundHeight);
}

SDL_Texture *getBackgroundTExture(SDL_Renderer *renderer){
    return textureBackground;
}


void drawBackground(SDL_Renderer *renderer, int camera_x, int camera_y){
    if (!textureBackground) return;

    int offset_x = camera_x % backgroundWidth;

    float scale = (float)SCREEN_HEIGHT / (float)backgroundHeight;
    int scaled_width = (int)(backgroundWidth * scale);


    SDL_Rect src = {offset_x, 0, backgroundWidth - offset_x, backgroundHeight};
    SDL_Rect dst = {0, 0, (int)((backgroundWidth - offset_x) * scale), SCREEN_HEIGHT};

    SDL_RenderCopy(renderer, textureBackground, &src, &dst);

    if (dst.w < SCREEN_WIDTH) {

        int remaining_width = SCREEN_WIDTH - dst.w;
        int src_width_needed = (int)(remaining_width / scale);


        SDL_Rect src2 = {0, 0, src_width_needed, backgroundHeight};
        SDL_Rect dst2 = {dst.w, 0, remaining_width, SCREEN_HEIGHT};

        SDL_RenderCopy(renderer, textureBackground, &src2, &dst2);
    }
}


void cleanupBackground(){
    if (textureBackground)
        SDL_DestroyTexture(textureBackground);
}
