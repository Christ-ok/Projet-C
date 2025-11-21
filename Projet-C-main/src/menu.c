#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h> // ajout pour malloc/free
#define WIDTH 1280
#define HEIGHT 720

SDL_Texture *LotDeTexture(SDL_Renderer * renderer, const char *path){
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture){
        SDL_Log("Erreur de chargement (%s): %s", path, IMG_GetError());
    }
    return texture;
}

void boucleEvenements(SDL_Renderer *renderer,
                      SDL_Texture *background,
                      SDL_Texture *titleTexture, SDL_Rect titleDst,
                      SDL_Texture **menuTextures, SDL_Rect *menuRects, int menuCount)
{
    SDL_Event e;
    int enCours = 1;
    int selected = -1;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    while (enCours){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                enCours = 0;
            } else if (e.type == SDL_MOUSEMOTION){
                int mx = e.motion.x, my = e.motion.y;
                selected = -1;
                for (int i = 0; i < menuCount; ++i){
                    if (mx >= menuRects[i].x && mx <= menuRects[i].x + menuRects[i].w &&
                        my >= menuRects[i].y && my <= menuRects[i].y + menuRects[i].h){
                        selected = i;
                        break;
                    }
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if (selected >= 0){
                    if (selected == 0) {
                        SDL_Log("Jouer sélectionné");
                        // action "JOUER" : remplacer par lancement de jeu
                    } else if (selected == 1) {
                        SDL_Log("Paramètres sélectionnés");
                        // action "PARAMÈTRES" : ouvrir menu paramètres
                    } else if (selected == 2) {
                        SDL_Log("Quitter sélectionné");
                        enCours = 0;
                    }
                }
            } else if (e.type == SDL_KEYDOWN){
                if (e.key.keysym.sym == SDLK_DOWN){
                    selected = (selected + 1) % menuCount;
                } else if (e.key.keysym.sym == SDLK_UP){
                    selected = (selected - 1 + menuCount) % menuCount;
                } else if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER){
                    if (selected >= 0){
                        if (selected == 0) { SDL_Log("Jouer (Enter)"); }
                        else if (selected == 1) { SDL_Log("Paramètres (Enter)"); }
                        else if (selected == 2) { enCours = 0; }
                    }
                } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                    enCours = 0;
                }
            }
        }

        // Rendu
        SDL_Rect dst = {0, 0, WIDTH, HEIGHT};
        if (background){
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background, NULL, &dst);
        } else {
            SDL_SetRenderDrawColor(renderer, 24, 40, 52, 255); // fallback couleur
            SDL_RenderClear(renderer);
        }

        // titre
        if (titleTexture) SDL_RenderCopy(renderer, titleTexture, NULL, &titleDst);

        // boutons : dessiner arrière-plan léger si survol
        for (int i = 0; i < menuCount; ++i){
            if (i == selected) {
                // rectangle semi-transparent derrière le texte
                SDL_SetRenderDrawColor(renderer, 0, 255, 170, 40); // verdâtre, alpha faible
                SDL_Rect r = menuRects[i];
                r.x -= 12; r.y -= 8; r.w += 24; r.h += 16;
                SDL_RenderFillRect(renderer, &r);
            }
            if (menuTextures[i]) SDL_RenderCopy(renderer, menuTextures[i], NULL, &menuRects[i]);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }
}

int main(int argc, char* argv[]) {

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if(0 != SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }

    if (TTF_Init() != 0) {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        goto Quit;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        fprintf(stderr, "Erreur IMG_Init: %s\n", IMG_GetError());
        goto Quit;
    }

    window = SDL_CreateWindow("Blade Quest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    
    if (NULL == window){
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (NULL == renderer){
        fprintf(stderr, "Erreur SDL_Createrenderer : %s", SDL_GetError());
        goto Quit;
    }
    
    if (0 != SDL_RenderClear(renderer))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }

    SDL_Texture *background = LotDeTexture(renderer, "c:\\Projet-C\\Projet-C-main\\assets\\background\\image_fondv1.png");

    // Chargement de la police TTF et création de la texture du titre
    TTF_Font *font_title = TTF_OpenFont("c:\\Projet-C\\Projet-C-main\\assets\\fonts\\Titre_menu.ttf", 128); // taille titre
    SDL_Texture *titleTexture = NULL;
    SDL_Rect titleDst = {0,0,0,0};
    if (!font_title) {
        fprintf(stderr, "Erreur chargement police: %s\n", TTF_GetError());
    } else {
        SDL_Color white = {255,255,255,255};
        SDL_Surface *surf = TTF_RenderUTF8_Blended(font_title, "Blade Quest", white);
        if (!surf) {
            fprintf(stderr, "Erreur TTF_RenderUTF8_Blended: %s\n", TTF_GetError());
        } else {
            titleTexture = SDL_CreateTextureFromSurface(renderer, surf);
            if (!titleTexture) {
                fprintf(stderr, "Erreur CreateTextureFromSurface: %s\n", SDL_GetError());
            } else {
                titleDst.w = surf->w;
                titleDst.h = surf->h;
                titleDst.x = (WIDTH - titleDst.w) / 2;
                titleDst.y = 140; // descendu un peu plus
            }
            SDL_FreeSurface(surf);
        }
    }

    // Création du menu (3 boutons)
    const char *items[] = {"JOUER", "PARAMETRES", "QUITTER"};
    const int menuCount = 3;
    SDL_Texture **menuTextures = malloc(sizeof(*menuTextures) * menuCount);
    SDL_Rect *menuRects = malloc(sizeof(*menuRects) * menuCount);
    if (!menuTextures || !menuRects) {
        fprintf(stderr, "Erreur allocation menu\n");
        goto Quit;
    }
    for (int i = 0; i < menuCount; ++i) {
        menuTextures[i] = NULL;
        menuRects[i] = (SDL_Rect){0,0,0,0};
    }

    TTF_Font *font_menu = TTF_OpenFont("c:\\Projet-C\\Projet-C-main\\assets\\fonts\\Titre_menu.ttf", 64); // taille menu
    if (!font_menu){
        fprintf(stderr, "Erreur chargement police menu: %s\n", TTF_GetError());
    } else {
        SDL_Color green = {50, 255, 180, 255};
        for (int i = 0; i < menuCount; ++i){
            SDL_Surface *s = TTF_RenderUTF8_Blended(font_menu, items[i], green);
            if (!s) {
                fprintf(stderr, "Erreur rendu texte menu: %s\n", TTF_GetError());
                continue;
            }
            menuTextures[i] = SDL_CreateTextureFromSurface(renderer, s);
            menuRects[i].w = s->w;
            menuRects[i].h = s->h;
            // positionner centré et espacé sous le titre
            int startY = titleDst.y + titleDst.h + 40;
            int spacing = 24 + s->h;
            menuRects[i].x = (WIDTH - menuRects[i].w) / 2;
            menuRects[i].y = startY + i * spacing;
            SDL_FreeSurface(s);
        }
    }

    boucleEvenements(renderer, background, titleTexture, titleDst, menuTextures, menuRects, menuCount);

Quit:
    for (int i = 0; i < menuCount; ++i) if (menuTextures && menuTextures[i]) SDL_DestroyTexture(menuTextures[i]);
    free(menuTextures);
    free(menuRects);
    if (font_menu) TTF_CloseFont(font_menu);
    if (titleTexture) SDL_DestroyTexture(titleTexture);
    if (font_title) TTF_CloseFont(font_title);
    if (background) SDL_DestroyTexture(background);
    if (NULL != renderer) SDL_DestroyRenderer(renderer);
    if (NULL != window) SDL_DestroyWindow(window);
      
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}