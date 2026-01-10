#include <stdio.h>
#include "blocs.h"
#include "background.h"
#include <SDL2/SDL_image.h>


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
}


void initBlocArray(Bloc *tab, int nb_blocs, int blocWidth, int blocHeight){
    int start_x = 600;
    int start_y = 330;
    int spacing = blocWidth;

    for (int i = 0; i < nb_blocs; i++){
        tab[i].x = start_x + (i * spacing);
        tab[i].y = start_y;
        tab[i].w = blocWidth;
        tab[i].h = blocHeight;
    }
}


void generateLevel(Bloc *level, int total_blocs, int blocWidth, int blocHeight){
    int current_x = 600;
    int current_y = 330;
    int platform_length = 10;
    int gap = 150;

    for (int i = 0; i < total_blocs; ){
        for (int j = 0; j < platform_length && i < total_blocs; j++, i++){
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
        SDL_RenderCopy(renderer, bloc, &src, &dst);
    }
}


void cleanupBloc(){
    if (bloc) SDL_DestroyTexture(bloc);
}


