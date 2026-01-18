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
static PauseButtons pauseButtons = {{0, 0, 0, 0}, {0, 0, 0, 0}};

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
            
            drawBackground(renderer, camera_x, jump_offset);
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

void renderPauseMenu(SDL_Renderer *renderer, int width, int height) {
    if (game_paused && pauseTexture) {
        SDL_RenderCopy(renderer, pauseTexture, NULL, NULL);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect overlay = {0, 0, width, height};
        SDL_RenderFillRect(renderer, &overlay);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect box = {width / 2 - 150, height / 2 - 120, 300, 240};
        SDL_RenderFillRect(renderer, &box);

        if (!pauseFont) {
            pauseFont = TTF_OpenFont("assets/fonts/Titre_menu.ttf", 48);
        }
        
        if (pauseFont) {
            SDL_Color white = {255, 255, 255, 255};
            
            SDL_Surface *textSurface = TTF_RenderText_Solid(pauseFont, "PAUSE", white);
            if (textSurface) {
                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                if (textTexture) {
                    SDL_Rect textRect = {
                        width / 2 - textSurface->w / 2,
                        height / 2 - 100,
                        textSurface->w,
                        textSurface->h
                    };
                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);
                }
                SDL_FreeSurface(textSurface);
            }

            TTF_Font *smallFont = TTF_OpenFont("assets/fonts/Titre_menu.ttf", 24);
            if (smallFont) {
                SDL_Surface *saveSurface = TTF_RenderText_Solid(smallFont, "SAUVEGARDER", white);
                if (saveSurface) {
                    pauseButtons.save_btn = (SDL_Rect){width / 2 - 80, height / 2 - 20, 160, 40};
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    SDL_RenderFillRect(renderer, &pauseButtons.save_btn);
                    
                    SDL_Texture *saveTexture = SDL_CreateTextureFromSurface(renderer, saveSurface);
                    if (saveTexture) {
                        SDL_Rect saveRect = {
                            width / 2 - saveSurface->w / 2,
                            height / 2 - saveSurface->h / 2,
                            saveSurface->w,
                            saveSurface->h
                        };
                        SDL_RenderCopy(renderer, saveTexture, NULL, &saveRect);
                        SDL_DestroyTexture(saveTexture);
                    }
                    SDL_FreeSurface(saveSurface);
                }

                SDL_Surface *quitSurface = TTF_RenderText_Solid(smallFont, "QUITTER", white);
                if (quitSurface) {
                    pauseButtons.quit_btn = (SDL_Rect){width / 2 - 80, height / 2 + 40, 160, 40};
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    SDL_RenderFillRect(renderer, &pauseButtons.quit_btn);
                    
                    SDL_Texture *quitTexture = SDL_CreateTextureFromSurface(renderer, quitSurface);
                    if (quitTexture) {
                        SDL_Rect quitRect = {
                            width / 2 - quitSurface->w / 2,
                            height / 2 + 40 + (40 - quitSurface->h) / 2,
                            quitSurface->w,
                            quitSurface->h
                        };
                        SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);
                        SDL_DestroyTexture(quitTexture);
                    }
                    SDL_FreeSurface(quitSurface);
                }

                TTF_CloseFont(smallFont);
            }
        }
    }
}

int handlePauseButtons(int mouse_x, int mouse_y, PauseButtons *buttons) {
    if (SDL_PointInRect(&(SDL_Point){mouse_x, mouse_y}, &buttons->save_btn)) {
        return 1; 
    }
    if (SDL_PointInRect(&(SDL_Point){mouse_x, mouse_y}, &buttons->quit_btn)) {
        return 2; 
    }
    return 0; 
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
