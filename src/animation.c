#include "animation.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Texture *idlePerso = NULL;
SDL_Texture *runPerso = NULL;
SDL_Texture *attackPerso = NULL;
SDL_Texture *JumpPerso = NULL;
SDL_Texture *DeathPerso = NULL;

int frameWidth = 96;
int frameHeight = 84;
int currentFrame = 0;
Uint32 lastFrameTime = 0;
const int frameTime = 120;

int locked = 0;
int currentState = 0;

void initCharacter(SDL_Renderer *renderer)
{
    idlePerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/IDLE.png");
    runPerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/RUN.png");
    attackPerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/ATTACK3.png");
    JumpPerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/JUMP.png");
    DeathPerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/DEATH.png");

    if (!idlePerso || !runPerso || !attackPerso || !JumpPerso || !DeathPerso)
        SDL_Log("Erreur chargement textures personnage");
}

void drawCharacter(SDL_Renderer *renderer, int x, int y, int state)
{
    Uint32 now = SDL_GetTicks();

    if (!locked)
    {
        if (state != currentState)
        {
            currentState = state;
            currentFrame = 0;

            if (state == 2 || state == 3 || state == 4)
                locked = 1;
        }
    }

    if (now - lastFrameTime >= frameTime)
    {
        currentFrame++;
        lastFrameTime = now;
    }

    SDL_Texture *Animation = idlePerso;
    int maxFrames = 7;

    if (currentState == 1)
    {
        Animation = runPerso;
        maxFrames = 8;
    }
    if (currentState == 2)
    {
        Animation = attackPerso;
        maxFrames = 6;
    }
    if (currentState == 3)
    {
        Animation = JumpPerso;
        maxFrames = 5;
    }
    if (currentState == 4)
    {
        Animation = DeathPerso;
        maxFrames = 12;
    }

    if (currentFrame >= maxFrames)
    {
        currentFrame = 0;

        if (locked)
        {
            locked = 0;
            currentState = 0;
        }
    }

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
    if (JumpPerso)
        SDL_DestroyTexture(JumpPerso);
    if (DeathPerso)
        SDL_DestroyTexture(DeathPerso);
}
