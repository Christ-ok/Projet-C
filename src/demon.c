#include "demon.h"
#include "animation.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>


#define WIDTH 800


SDL_Texture *idleDemon = NULL;
SDL_Texture *attackDemon = NULL;
SDL_Texture *HurtDemon = NULL;
SDL_Texture *DeathDemon = NULL;

int Demon_frameWidth = 79;
int Demon_frameHeight = 69;
const int Demon_frameTime = 120;

void initDemon(SDL_Renderer *renderer)
{
    idleDemon = IMG_LoadTexture(renderer, "./assets/tiles/Characters/demon/IDLE.png");
    attackDemon = IMG_LoadTexture(renderer, "./assets/tiles/Characters/demon/ATTACK.png");
    HurtDemon = IMG_LoadTexture(renderer, "./assets/tiles/Characters/demon/HURT.png");
    DeathDemon = IMG_LoadTexture(renderer, "./assets/tiles/Characters/demon/DEATH.png");

    if (!idleDemon || !attackDemon || !HurtDemon || !DeathDemon)
        SDL_Log("Erreur chargement textures demon");
}

void Demon_spawn(Demon *d, int x, int y)
{
    d->x = x;
    d->y = y;
    d->y_base = y;
    d->x_base = x;
    d->HP = 8;
    d->dead = 0;
    d->exists = 1;
    d->currentState = 0;
    d->currentFrame = 0;
    d->locked = 0;
    d->lastFrameTime = SDL_GetTicks();
    d->isLeft = 0;
}




void Demon_takeDamage(Demon *d, int damage)
{

    if (d->exists == 0)
        return;

    d->HP -= damage;

    if (d->HP <= 0)
    {
        d->HP = 0;
        d->dead = 1;

        d->currentState = 7;
        d->currentFrame = 0;
        d->locked = 1;
    }
    else
    {
        d->currentState = 2;
        d->currentFrame = 0;
        d->locked = 1;
    }
}

void drawDemon(SDL_Renderer *renderer, Demon *d, int state)
{
    if (d->exists == 0)
        return;

    SDL_RendererFlip Demon_flip = d->isLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    Uint32 Demon_now = SDL_GetTicks();

    if (!d->locked)
    {
        if (state != d->currentState)
        {
            d->currentFrame = 0;
            d->currentState = state;

            if (state == 2 || state == 3 || state == 4 || state == 5)
            {
                d->locked = 1;
            }
        }
    }

    if (Demon_now - d->lastFrameTime >= Demon_frameTime)
    {
        d->currentFrame++;
        d->lastFrameTime = Demon_now;
    }

    SDL_Texture *Demon_Animation = idleDemon;
    int Demon_maxFrames = 4;

    if (d->currentState == 5)
    {
        Demon_Animation = attackDemon;
        Demon_maxFrames = 8;
    }
    if (d->currentState == 2)
    {
        Demon_Animation = HurtDemon;
        Demon_maxFrames = 4;
    }
    if (d->currentState == 7)
    {
        Demon_Animation = DeathDemon;
        Demon_maxFrames = 7;
    }

    if (d->currentFrame >= Demon_maxFrames)
    {

        if (d->currentState == 7)
        {
            d->exists = 0;
            return;
        }

        d->currentFrame = 0;

        if (d->locked)
        {
            d->locked = 0;
            d->currentState = 0;
        }
    }

    SDL_Rect Demon_src = {d->currentFrame * Demon_frameWidth, 0, Demon_frameWidth, Demon_frameHeight};
    SDL_Rect Demon_dst = {d->x, d->y, Demon_frameWidth * 1.75, Demon_frameHeight * 1.75};

    SDL_RenderCopyEx(renderer, Demon_Animation, &Demon_src, &Demon_dst, 0, NULL, Demon_flip);
}


void updateDemon(Demon *d, int d_pressed, int q_pressed, int space_pressed, int *camera_x, int *camera_y, Personnage *p){
    if (d_pressed) d->x -= p->speed;
    if (q_pressed) d->x += p->speed;

    *camera_x = p->x - WIDTH / 4;
    if (*camera_x < 0) d->x = d->x_base;


    /*
        void cameraY(int *camera_x, int *camera_y, Personnage *p, int jump_offset, int screen_width){

            *camera_x = p->x - WIDTH / 2;
            if (*camera_x < 0) *camera_x = 0;
    
            *camera_y = (jump_offset / 2);
        }
    */

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
