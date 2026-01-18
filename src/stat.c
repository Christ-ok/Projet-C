#include "demon.h"
#include "animation.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

SDL_Texture *ImageDemon = NULL;
SDL_Texture *Image_HP = NULL;
static TTF_Font *statFont = NULL;

int Image_frameWidth = 79;
int Image_frameHeight = 69;

int HP_frameWidth = 15;
int HP_frameHeight = 24;

void initImageDemon(SDL_Renderer *renderer)
{
    Image_HP = IMG_LoadTexture(renderer, "./assets/tiles/tile_0044.png");
    ImageDemon = IMG_LoadTexture(renderer, "./assets/tiles/Characters/demon/IDLE.png");
    statFont = TTF_OpenFont("assets/fonts/ARIAL.TTF", 40);

    if (!ImageDemon)
        SDL_Log("Erreur chargement textures demon");
    if (!Image_HP)
        SDL_Log("Erreur chargement textures demon");
    if (!statFont)
        printf("Erreur chargement font stat: %s\n", TTF_GetError());
}

void stat(SDL_Renderer *renderer, int demonCount)
{
    char statText[16];
    sprintf(statText, "%d / 10", demonCount);

    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *textSurface = TTF_RenderText_Solid(statFont, statText, white);
    if (!textSurface)
        return;

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!textTexture)
        return;

    SDL_Rect textRect;
    textRect.x = 580;
    textRect.y = 10;
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

void statCoeur(SDL_Renderer *renderer, Personnage *p)
{
    char statText[16];
    sprintf(statText, "%d HP", p->HP);

    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *textSurface = TTF_RenderText_Solid(statFont, statText, white);
    if (!textSurface)
        return;

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!textTexture)
        return;

    SDL_Rect textRect;
    textRect.x = 100;
    textRect.y = 10;
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

void drawDemonStat(SDL_Renderer *renderer)
{
    if (!ImageDemon)
        return;

    SDL_Rect demonRect;
    demonRect.x = 700;
    demonRect.y = -10;
    demonRect.w = Image_frameWidth;
    demonRect.h = Image_frameHeight;

    SDL_Rect Demon_src = {1, 0, Image_frameWidth, Image_frameHeight};
    SDL_Rect Demon_dst = {demonRect.x, demonRect.y, Image_frameWidth * 1.25, Image_frameHeight * 1.25};

    SDL_RenderCopy(renderer, ImageDemon, &Demon_src, &Demon_dst);
}

void drawCoeurStat(SDL_Renderer *renderer)
{
    if (!Image_HP)
        return;

    SDL_Rect HP_Rect;
    HP_Rect.x = 10;
    HP_Rect.y = 0;
    HP_Rect.w = HP_frameHeight;
    HP_Rect.h = HP_frameHeight;

    SDL_Rect HP_src = {1, 0, Image_frameWidth, Image_frameHeight};
    SDL_Rect HP_dst = {HP_Rect.x, HP_Rect.y, Image_frameWidth * 1, Image_frameHeight * 1};

    SDL_RenderCopy(renderer, Image_HP, &HP_src, &HP_dst);
}

void cleanUpImageDemon()
{
    if (ImageDemon)
        SDL_DestroyTexture(ImageDemon);
}
void cleanUpImageHP()
{
    if (Image_HP)
        SDL_DestroyTexture(Image_HP);
}