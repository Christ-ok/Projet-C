#include "animation.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Texture *idlePerso = NULL;
SDL_Texture *runPerso = NULL;
SDL_Texture *attackPerso = NULL;
SDL_Texture *JumpPerso = NULL;
SDL_Texture *DeathPerso = NULL;
SDL_Texture *HurtPerso = NULL;

int frameWidth = 96;
int frameHeight = 84;
int currentFrame = 0;
Uint32 lastFrameTime = 0;
const int frameTime = 120;

int locked = 0;
int currentState = 0;

#define SCREENHEIGT 600

void initCharacter(SDL_Renderer *renderer, Personnage *p)
{
    idlePerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/IDLE.png");
    runPerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/RUN.png");
    attackPerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/ATTACK3.png");
    JumpPerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/JUMP.png");
    DeathPerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/DEATH.png");
    HurtPerso = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/HURT.png");

    if (!idlePerso || !runPerso || !attackPerso || !JumpPerso || !DeathPerso || !HurtPerso)
        SDL_Log("Erreur chargement textures personnage");

    p->HP = 5;
    p->alive = 1;
    p->dead = 0;
    p->x = 150;
    p->y = 200;
    p->base_y = 200;
    p->w = 96;
    p->h = 84;
    p->speed = 10;
    p->state = 0;
    p->direction = 0;
    p->on_ground = 1;
}

void Hero_takeDamage(Personnage *p, int damage)
{

    if (p->alive == 0)
        return;

    p->HP -= damage;

    if (p->HP <= 0)
    {
        p->HP = 0;
        p->alive = 0;
        currentState = 4;
        currentFrame = 0;
        locked = 1;
    }
    else
    {
        currentState = 5;
        currentFrame = 0;
        locked = 1;
    }
}

void drawCharacter(SDL_Renderer *renderer, Personnage *p, int camera_x, int camera_y, int jump_offset)
{
    /*
    if (p->alive == 0)
    {
        currentFrame = 12;
        return;
    }
    */

    SDL_RendererFlip flip = p->direction ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    Uint32 now = SDL_GetTicks();

    if (!locked)
    {
        if (p->state != currentState)
        {
            if (p->state == 5)
            {
                locked = 1;
                return;
            }

            currentFrame = 0;
            currentState = p->state;

            if (p->state == 2 || p->state == 3 || p->state == 4 || p->state == 5)
            {
                locked = 1;
            }
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
    if (currentState == 5)
    {
        Animation = HurtPerso;
        maxFrames = 4;
    }

    if (currentFrame >= maxFrames)
    {

        if (currentState == 4)
        {
            currentFrame = maxFrames - 1;
        }

        else
        {

            currentFrame = 0;

            if (locked)
            {
                locked = 0;

                if (currentState != 1)
                {
                    currentState = 0;
                }
            }
        }
    }

    SDL_Rect src = {currentFrame * frameWidth, 0, frameWidth, frameHeight};
    SDL_Rect dst = {p->x - camera_x, (p->base_y - jump_offset) - camera_y, frameWidth * 4, frameHeight * 4};

    SDL_RenderCopyEx(renderer, Animation, &src, &dst, 0, NULL, flip);
}

void updateCharacter(Personnage *p, int d_pressed, int q_pressed, int space_pressed, int camera_x)
{
    if (p->alive == 0)
    {
        return;
    }
    if (d_pressed)
        p->x += p->speed;
    if (q_pressed)
        p->x -= p->speed;
    if (space_pressed == 1)
        p->y += p->speed;
    if (space_pressed == 0)
    {
    }

    if (p->x < 0)
        p->x = camera_x;
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
    if (HurtPerso)
        SDL_DestroyTexture(HurtPerso);
}
