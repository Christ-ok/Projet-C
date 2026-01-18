#include <stdio.h>
#include "blocs.h"
#include "background.h"
#include "animation.h"
#include <SDL2/SDL_image.h>

#define SPEED 5

SDL_Texture *bloc = NULL;
int blocWidth = 0;
int blocHeight = 0;

void initBloc(SDL_Renderer * renderer, Bloc *b){
    bloc = IMG_LoadTexture(renderer, "assets/tiles/Backgrounds/tile_0013.png");
    if (!bloc) SDL_Log("Erreur lors du chargement du bloc: %s", IMG_GetError());

    SDL_QueryTexture(bloc, NULL, NULL, &blocWidth, &blocHeight);

    b->x = 500;
    b->y = 500;
    b->w = blocWidth; 
    b->h = blocHeight; 
    b->ground = 0;
    b->transparant = 0;
}


void initBlocArray(Bloc *tab, int nb_blocs, int blocWidth, int blocHeight){
    int start_x = 300; 
    int start_y = 350; 
    int spacing = blocWidth;

    for (int i = 0; i < nb_blocs; i++){
        tab[i].x = start_x + (i * spacing);
        tab[i].y = start_y;
        tab[i].w = blocWidth;
        tab[i].h = blocHeight;
    }
}


void generateLevel(Bloc *level, int total_blocs, int blocWidth, int blocHeight){

    int current_x = 300;
    int current_y = 350;
    int platform_length = 10;
    int gap = 200;

    for (int i = 0; i < total_blocs; i++){
        for (int j = 0; j < platform_length && i < total_blocs; j++){
            level[i].x = current_x;
            level[i].y = current_y;
            level[i].w = blocWidth;
            level[i].h = blocHeight;

            current_x += blocWidth;
        }

        current_x += gap;
    }   
}


void drawBloc(SDL_Renderer *renderer, Bloc *tab, int nb_blocs, int camera_x){

    SDL_Rect src = {0, 0, blocWidth, blocHeight};

    for (int i = 0; i < nb_blocs; i++)
    {
        SDL_Rect dst = {tab[i].x - camera_x, tab[i].y, tab[i].w, tab[i].h};
        SDL_RenderCopy(renderer, bloc, NULL, &dst);
    }
}


void applyGravity(Personnage *p){
    const int gravity = 7;

    if (p->on_ground == 0)
    {
        p->base_y += gravity;
    }
}


int verifyRectSuperposition(Personnage *p, Bloc *b, int jump_offset){
    int superposition;

    int perso_y =p->base_y - jump_offset;
    int perso_bottom = perso_y + (p->h * 4);
    int perso_x = p->x;
    int perso_right = p->x + (p->w * 4);

    int bloc_top = b->y;
    int bloc_bottom = b->y + b->h;
    int bloc_left = b->x;
    int bloc_right = b->x + b->w;

    if (perso_y < bloc_bottom && perso_x > bloc_top)
    {                            
        superposition = 1;
    } else {
        superposition = 0;
    }

    return superposition;
}


int verifyRectOnEmpty(Personnage *p, Bloc *b, int jump_offset){
    int on_empty;

    int perso_y =p->base_y - jump_offset;
    int perso_bottom = perso_y + (p->h * 4);
    int perso_x = p->x;
    int perso_right = p->x + (p->w * 4);

    int bloc_top = b->y;
    int bloc_bottom = b->y + b->h;
    int bloc_left = b->x;
    int bloc_right = b->x + b->w;

    if (verifyRectSuperposition(p, b, jump_offset)){
        on_empty = 0;
    } else {
        on_empty = 1;
    }

    return on_empty;
}


void checkCollisionWithBlocs(Bloc *level, int nb_blocs, Personnage *p, int jump_offset, int *jump_direction){
    p->on_ground = 0;
    
    int perso_y =p->base_y - jump_offset;
    int perso_bottom = perso_y + (p->h * 4);
    int perso_x = p->x;
    int perso_right = p->x + (p->w * 4);
    
    
    const int GROUND_LEVEL = 650;
    if (perso_bottom >= GROUND_LEVEL)
    {
        p->on_ground = 1;
        p->base_y = GROUND_LEVEL - (p->h * 4);
        return;
    }
    
    for (int i = 0; i < nb_blocs; i++)
    {
        int bloc_top = level[i].y;
        int bloc_bottom = level[i].y + level[i].h;
        int bloc_left = level[i].x;
        int bloc_right = level[i].x + level[i].w;

        int collision_x;

        if (perso_right > bloc_left && perso_x < bloc_right)
        {
            collision_x = 1;
        } else
        {
            collision_x = 0;
        }



        int collision_y;

        if (verifyRectSuperposition(p, level, jump_offset)){
            collision_y = 1;
        } else {
            collision_y = 0;
        }
       

        if (collision_x && collision_y)
        {
            p->on_ground = 1;
            p->base_y = bloc_top - (p->h * 2.9);
            return;
        } else if (verifyRectOnEmpty(p, level, jump_offset))
        {
            p->on_ground = 0;
            if (p->on_ground == 0){
                if (verifyRectOnEmpty(p, level, jump_offset) == 1){
                    if (perso_x > bloc_right){
                        if (*jump_direction == -1){
                            p->base_y = bloc_top - (p->h * 2);
                        }
                    }
                }
            }
        } 
    }
}


void cleanupBloc(){
    if (bloc) SDL_DestroyTexture(bloc);
}


