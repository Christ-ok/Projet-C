#include "gameover.h"
#include <stdio.h>

static TTF_Font *gameOverFont = NULL;

void GameOver(SDL_Renderer *renderer, int width, int height) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &overlay);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    if (!gameOverFont) {
        gameOverFont = TTF_OpenFont("assets/fonts/Titre_menu.ttf", 48);
        if (!gameOverFont) printf("Erreur chargement font : %s\n", TTF_GetError());
    }

    if (gameOverFont) {
        SDL_Color red = {255, 0, 0, 255};
        SDL_Color white = {255, 255, 255, 255};

        SDL_Surface *textSurface = TTF_RenderText_Solid(gameOverFont, "GAME OVER", red);
        if (textSurface) {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                SDL_Rect textRect = {width / 2 - textSurface->w / 2, height / 2 - 100, textSurface->w, textSurface->h};
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }


        textSurface = TTF_RenderText_Solid(gameOverFont, "QUITTER", white);
        if (textSurface) {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textSurface) {
                SDL_Rect textRect = {width / 2 - textSurface->w / 2, height /2 + 30 + (40- textSurface->h) / 2, textSurface->w, textSurface->h};
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
                }
            SDL_FreeSurface(textSurface);
            }
        }    
    }

int handleGameOverButtons(int mouseX, int mouseY, int width, int height) {
    SDL_Rect quitRect = {width / 2 - 80, height / 2 + 20, 160, 40};
    SDL_Point mousePoint = {mouseX, mouseY};
    if (SDL_PointInRect(&mousePoint, &quitRect)) {
        return 1; 
    }
    return 0;
}

void cleanupGameOver() {
    if (gameOverFont) {
        TTF_CloseFont(gameOverFont);
        gameOverFont = NULL;
    }
}

void GameFinished(SDL_Renderer *renderer, int width, int height) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &overlay);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_AllocateChannels(16);

    if (!gameOverFont) {
        gameOverFont = TTF_OpenFont("assets/fonts/Titre_menu.ttf", 48);
        if (!gameOverFont) printf("Erreur chargement font : %s\n", TTF_GetError());
    }

    if (gameOverFont) {
        SDL_Color green = {0,128,0,255};
        SDL_Color white = {255, 255, 255, 255};

        SDL_Surface *textSurface = TTF_RenderText_Solid(gameOverFont, "NIVEAU TERMINE", green);
        if (textSurface) {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                SDL_Rect textRect = {width / 2 - textSurface->w / 2, height / 2 - 100, textSurface->w, textSurface->h};
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }

        textSurface = TTF_RenderText_Solid(gameOverFont, "QUITTER", white);
        if (textSurface) {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textSurface) {
                SDL_Rect textRect = {width / 2 - textSurface->w / 2, height /2 + 30 + (40- textSurface->h) / 2, textSurface->w, textSurface->h};
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
                }
            SDL_FreeSurface(textSurface);
            }
        }    
    Mix_Music *winsound = Mix_LoadMUS("assets/audio/win_sound.ogg");
    if (winsound)
    {
        Mix_VolumeMusic((10 * MIX_MAX_VOLUME) / 100);
        Mix_PlayMusic(winsound, -1);
    }

    if (winsound)
        Mix_FreeMusic(winsound);
    Mix_CloseAudio();
    Mix_Quit();
}