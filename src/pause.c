#include "pause.h"
#include "background.h"
#include "animation.h"
#include "demon.h"
#include "deplacement2.h"
#include <stdio.h>
#include <SDL2/SDL_ttf.h>
#define MAX_DEMONS 7

static int game_paused = 0;
static SDL_Texture *pauseTexture = NULL;
static TTF_Font *pauseFont = NULL;

void togglePause(SDL_Renderer *renderer, int width, int height, int camera_x, int camera_y, int jump_offset, int state, int gauche, int PlayerIsLeft, Personnage *perso, Demon *horde) {
    game_paused = !game_paused;

    if (game_paused) {
        if (pauseTexture) {
            SDL_DestroyTexture(pauseTexture);
        }
        pauseTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        
        if (pauseTexture) {
            SDL_SetRenderTarget(renderer, pauseTexture);
            SDL_RenderClear(renderer);
            
            drawBackground(renderer, camera_x, camera_y);
            drawCharacter(renderer, perso, camera_x, camera_y, jump_offset);
            for (int i = 0; i < MAX_DEMONS; i++)
                {
                    if (horde[i].exists)
                    {
                    drawDemon(renderer, &horde[i], horde[i].currentState);
                    }
                }            
            SDL_SetRenderTarget(renderer, NULL);
        }
    } else {
        if (pauseTexture) {
            SDL_DestroyTexture(pauseTexture);
            pauseTexture = NULL;
        }
    }
}

int isGamePaused() {
    return game_paused;
}

int handlePauseButtons(int mouseX, int mouseY, PauseButtons *buttons) {
    SDL_Point mousePoint = {mouseX, mouseY};
    if (SDL_PointInRect(&mousePoint, &buttons->save_btn)) {
        return 1; 
    }
    if (SDL_PointInRect(&mousePoint, &buttons->quit_btn)) {
        return 2; 
    }
    return 0;
}

void renderPauseMenu(SDL_Renderer *renderer, int width, int height) {
    if (game_paused && pauseTexture) {
        SDL_RenderCopy(renderer, pauseTexture, NULL, NULL);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect overlay = {0, 0, width, height};
        SDL_RenderFillRect(renderer, &overlay);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect box = {width / 2 - 150, height / 2 - 100, 300, 200};
        SDL_RenderFillRect(renderer, &box);

        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255); 
        SDL_Rect saveRect = {width / 2 - 80, height / 2 - 20, 160, 40};
        SDL_RenderFillRect(renderer, &saveRect);
        
        SDL_Rect quitRect = {width / 2 - 80, height / 2 + 40, 160, 40};
        SDL_RenderFillRect(renderer, &quitRect);

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); 
        SDL_RenderDrawRect(renderer, &saveRect);
        SDL_RenderDrawRect(renderer, &quitRect);

        if (!pauseFont) {
            pauseFont = TTF_OpenFont("assets/fonts/Titre_menu.ttf", 48);
            if (!pauseFont) printf("Erreur chargement font pause: %s\n", TTF_GetError());
        }
        
        if (pauseFont) {
            SDL_Color white = {255, 255, 255, 255};
            SDL_Surface *textSurface = TTF_RenderText_Solid(pauseFont, "PAUSE", white);
            if (textSurface) {
                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                if (textTexture) {
                    SDL_Rect textRect = {
                        width / 2 - textSurface->w / 2,
                        height / 2 - 80,
                        textSurface->w,
                        textSurface->h
                    };
                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);
                }
                SDL_FreeSurface(textSurface);
            }

            textSurface = TTF_RenderText_Solid(pauseFont, "SAUVEGARDER", white);
            if (textSurface) {
                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                if (textTexture) {
                    SDL_Rect textRect = {width / 2 - textSurface->w / 2, height / 2 - 20 + (40 - textSurface->h) / 2, textSurface->w, textSurface->h};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);
                }
                SDL_FreeSurface(textSurface);
            }

            textSurface = TTF_RenderText_Solid(pauseFont, "QUITTER", white);
            if (textSurface) {
                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                if (textTexture) {
                    SDL_Rect textRect = {width / 2 - textSurface->w / 2, height / 2 + 40 + (40 - textSurface->h) / 2, textSurface->w, textSurface->h};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);
                }
                SDL_FreeSurface(textSurface);
            }
        }
    }
}

void cleanupPause() {
    if (pauseTexture) {
        SDL_DestroyTexture(pauseTexture);
        pauseTexture = NULL;
    }
    if (pauseFont) {
        TTF_CloseFont(pauseFont);
        pauseFont = NULL;
    }
}
