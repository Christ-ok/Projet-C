#include "plateforme.h"
#include "animation.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define WIDTH 800
#define MAX_PLATEFORMES 4

Plateforme plateformes[MAX_PLATEFORMES];

SDL_Texture *PlateformeTexture = NULL;

void initPlateformeTexture(SDL_Renderer *renderer)
{
    PlateformeTexture = IMG_LoadTexture(renderer, "assets/tiles/Backgrounds/tile_0013.png");
    if (!PlateformeTexture)
        SDL_Log("Erreur chargement texture plateforme : %s", SDL_GetError());
}

void spawn_Plateforme(Plateforme *Ptf, int x, int y, int tiles_x, int tiles_y)
{
    Ptf->x = x;
    Ptf->y = y;
    Ptf->x_base = x;
    Ptf->y_base = y;
    Ptf->tiles_x = tiles_x;
    Ptf->tiles_y = tiles_y;
    Ptf->w = tiles_x * TILE_SIZE;
    Ptf->h = tiles_y * TILE_SIZE;
    Ptf->ground = 0;
}

void Plateforme_draw(SDL_Renderer *renderer, Plateforme *Ptf, int camera_x)
{
    SDL_Rect dst;
    dst.w = TILE_SIZE;
    dst.h = TILE_SIZE;

    for (int y = 0; y < Ptf->tiles_y; y++)
    {
        for (int x = 0; x < Ptf->tiles_x; x++)
        {
            dst.x = Ptf->x - camera_x + x * TILE_SIZE;
            dst.y = Ptf->y + y * TILE_SIZE;
            SDL_RenderCopy(renderer, PlateformeTexture, NULL, &dst);
        }
    }
}

int verifyRectSuperposition(Personnage *p, Plateforme *Pft, int jump_offset)
{
    int perso_y = p->base_y - jump_offset;
    int perso_bottom = perso_y + (p->h * 4);
    int perso_x = p->x;
    int perso_right = perso_x + (p->w * 4);

    int bloc_top = Pft->y;
    int bloc_bottom = Pft->y + Pft->h;
    int bloc_left = Pft->x;
    int bloc_right = Pft->x + Pft->w;

    if (perso_right > bloc_left &&
        perso_x < bloc_right &&
        perso_bottom > bloc_top &&
        perso_y < bloc_bottom)
    {
        return 1;
    }
    return 0;
}

void checkCollisionWithPlateform(Plateforme *Ptf, Personnage *p, int jump_offset, int *jump_direction)
{
    int perso_y = p->base_y - jump_offset;
    int perso_bottom = perso_y + (p->h * 4);
    int perso_x = p->x;
    int perso_right = perso_x + (p->w * 4);

    p->on_ground = 0;

    const int GROUND_LEVEL = 630;
    if (perso_bottom >= GROUND_LEVEL)
    {
        p->on_ground = 1;
        p->base_y = GROUND_LEVEL - (p->h * 4);
        *jump_direction = 0;
        return;
    }

    for (int i = 0; i < MAX_PLATEFORMES; i++)
    {
        Plateforme *plat = &plateformes[i];

        int bloc_top = plat->y;
        int bloc_left = plat->x;
        int bloc_right = plat->x + plat->w;

        int collision_x = (perso_right > bloc_left && perso_x < bloc_right);

        if (collision_x &&
            perso_bottom >= bloc_top &&
            perso_bottom <= bloc_top + 12 &&
            *jump_direction == -1)
        {
            p->on_ground = 1;
            p->base_y = bloc_top - (p->h * 4);
            *jump_direction = 0;
            return;
        }
    }
}

void cleanUpPlateforme()
{
    if (PlateformeTexture)
        SDL_DestroyTexture(PlateformeTexture);
}
