#include "animation.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Texture *idlePerso = NULL;
SDL_Texture *runPerso = NULL;
SDL_Texture *attackPerso = NULL;

int frameWidth = 96;
int frameHeight = 84;
int currentFrame = 0;
Uint32 lastFrameTime = 0;
const int frameTime = 120;

void initCharacter(SDL_Renderer *renderer)
{
    idlePerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/IDLE.png");
    runPerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/RUN.png");
    attackPerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/ATTACK3.png");

    if (!idlePerso || !runPerso || !attackPerso)
        SDL_Log("Erreur chargement textures personnage");
}

void drawCharacter(SDL_Renderer *renderer, int x, int y, int state)
{
    Uint32 now = SDL_GetTicks();
    if (now - lastFrameTime >= frameTime)
    {
        if (state == 2)
        {

            currentFrame = (currentFrame + 1) % 6;
            lastFrameTime = now;
        }
        currentFrame++;
        lastFrameTime = now;
    }

    SDL_Texture *Animation = idlePerso;
    int maxFrames = 7;
    if (state == 1)
    {
        Animation = runPerso;
        maxFrames = 8;
    }
    if (state == 2)
    {
        Animation = attackPerso;
        maxFrames = 6;
    }
    currentFrame %= maxFrames;

    SDL_Rect src = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
    SDL_Rect dst = {x - frameWidth * 2, y - frameHeight * 2, frameWidth * 4, frameHeight * 4};
    SDL_RenderCopy(renderer, Animation, &src, &dst);
}

void cleanupCharacter()
{
    if (idlePerso)
        SDL_DestroyTexture(idlePerso);
    if (runPerso)
        SDL_DestroyTexture(runPerso);
    if (attackPerso)
        SDL_DestroyTexture(attackPerso);
}
