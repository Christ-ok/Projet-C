#include "demon.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Texture *idleDemon = NULL;
SDL_Texture *attackDemon = NULL;
SDL_Texture *HurtDemon = NULL;
SDL_Texture *DeathDemon = NULL;

int Demon_frameWidth = 79;
int Demon_frameHeight = 69;
int Demon_currentFrame = 0;
Uint32 Demon_lastFrameTime = 0;
const int Demon_frameTime = 120;

int Demon_locked = 0;
int Demon_currentState = 0;

const int Demon_MaxPV = 5;
int Demon_HP = Demon_MaxPV;
int Demon_dead = 0;

int Demon_exists = 1;

void initDemon(SDL_Renderer *renderer)
{
    idleDemon = IMG_LoadTexture(renderer, "./assets/tiles/Characters/demon/IDLE.png");
    attackDemon = IMG_LoadTexture(renderer, "./assets/tiles/Characters/demon/ATTACK.png");
    HurtDemon = IMG_LoadTexture(renderer, "./assets/tiles/Characters/demon/HURT.png");
    DeathDemon = IMG_LoadTexture(renderer, "./assets/tiles/Characters/demon/DEATH.png");

    if (!idleDemon || !attackDemon || !HurtDemon || !DeathDemon)
        SDL_Log("Erreur chargement textures demon");
}

void Demon_takeDamage(int damage)
{
    if (Demon_exists == 0)
        return;

    Demon_HP -= damage;

    if (Demon_HP <= 0)
    {
        Demon_HP = 0;
        Demon_dead = 1;

        Demon_currentState = 7;
        Demon_currentFrame = 0;
        Demon_locked = 1;
    }
    else
    {
        Demon_currentState = 2;
        Demon_currentFrame = 0;
        Demon_locked = 1;
    }
}

void drawDemon(SDL_Renderer *renderer, int x, int y, int state, int PlayerIsLeft)
{
    if (Demon_exists == 0)
        return;

    SDL_RendererFlip Demon_flip = PlayerIsLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    Uint32 Demon_now = SDL_GetTicks();

    if (!Demon_locked)
    {
        if (state != Demon_currentState)
        {
            Demon_currentFrame = 0;
            Demon_currentState = state;

            if (state == 2 || state == 3 || state == 4 || state == 5)
            {
                Demon_locked = 1;
            }
        }
    }

    if (Demon_now - Demon_lastFrameTime >= Demon_frameTime)
    {
        Demon_currentFrame++;
        Demon_lastFrameTime = Demon_now;
    }

    SDL_Texture *Demon_Animation = idleDemon;
    int Demon_maxFrames = 4;

    if (Demon_currentState == 5)
    {
        Demon_Animation = attackDemon;
        Demon_maxFrames = 8;
    }
    if (Demon_currentState == 2)
    {
        Demon_Animation = HurtDemon;
        Demon_maxFrames = 4;
    }
    if (Demon_currentState == 7)
    {
        Demon_Animation = DeathDemon;
        Demon_maxFrames = 7;
    }

    if (Demon_currentFrame >= Demon_maxFrames)
    {

        if (Demon_currentState == 7)
        {
            Demon_exists = 0;
            return;
        }

        Demon_currentFrame = 0;

        if (Demon_locked)
        {
            Demon_locked = 0;
            Demon_currentState = 0;
        }
    }

    SDL_Rect Demon_src = {Demon_currentFrame * Demon_frameWidth, 0, Demon_frameWidth, Demon_frameHeight};
    SDL_Rect Demon_dst = {x - Demon_frameWidth * -2, y - Demon_frameHeight * 1, Demon_frameWidth * 2, Demon_frameHeight * 2};

    SDL_RenderCopyEx(renderer, Demon_Animation, &Demon_src, &Demon_dst, 0, NULL, Demon_flip);
}

void cleanupDemon()
{
    if (idleDemon)
        SDL_DestroyTexture(idleDemon);
    if (attackDemon)
        SDL_DestroyTexture(attackDemon);
    if (HurtDemon)
        SDL_DestroyTexture(HurtDemon);
    if (DeathDemon)
        SDL_DestroyTexture(DeathDemon);
}
