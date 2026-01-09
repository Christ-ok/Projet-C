#ifndef MENU_OPTI_H
#define MENU_OPTI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct
{
    SDL_Texture **items;
    SDL_Rect *rects;
    int count;
    int selected;
} Menu;

typedef enum
{
    MENU_MAIN,
    MENU_JOUER,
    MENU_PARAM,
    MENU_AUDIO,
    MENU_RESOL
} MenuState;

SDL_Texture *create_text(SDL_Renderer *r, TTF_Font *f, const char *txt, SDL_Color c, int *ow, int *oh);
Menu menu_create(SDL_Renderer *renderer, TTF_Font *font, const char **labels, int count);
void menu_free(Menu *m);
void menu_layout(Menu *m, int winW, int winH, int startY);
void menu_handle_mouse(Menu *m, int mx, int my);
void menu_render(SDL_Renderer *renderer, Menu *m);
void save_config(int vol, int w, int h, int fs);
void load_config(int *vol, int *w, int *h, int *fs);

#endif