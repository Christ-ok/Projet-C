#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#define WIDTH 800
#define HEIGHT 600
#define FPS_LIMIT 16


//Fonction pour controler la vitesse d'affichage d'images dans le programme
void SDL_LimitFPS(unsigned int limit){

    unsigned int ticks = SDL_GetTicks();

    if (limit < ticks){
        return;
    } else if(limit > ticks + FPS_LIMIT){
        SDL_Delay(FPS_LIMIT);
    }else{
        SDL_Delay(limit - ticks);
    }
    
}



SDL_Texture *LotDeTexture(SDL_Renderer * renderer, const char *path){
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture){
        SDL_Log("Erreur de chargement (%s): %s", path, IMG_GetError());
    }

    return texture;
}


SDL_Texture **loadTabTexture(SDL_Renderer *renderer, char *PathFolder, const char *extension, int taille){
    SDL_Texture **tabPersos = malloc(sizeof(SDL_Texture *) * taille);

    if(!tabPersos){
        perror("Erreur d'allocation sur le tableau de personnages");
        exit(1);
    }

    for (int i = 0; i < taille; i++){
        char complete_path[300];

        // Chemin propre et sécurisé
        sprintf(complete_path, "%s%04d%s", PathFolder, i, extension);

        SDL_Texture *texture = LotDeTexture(renderer, complete_path);

        if (!texture){
            printf("Impossible de charger : %s\n", complete_path);
        }

        tabPersos[i] = texture;
    }
    
    return tabPersos;
}






int main(int argc, char *argv[]){

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;    
    SDL_Texture *texture2 = NULL;
    SDL_Surface *surface = NULL;
    int statut = EXIT_FAILURE;
    unsigned int frame_limit = 0;
    
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    
    window = SDL_CreateWindow("BLADE QUEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    
    if (window == NULL){
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL){
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }
    
    
    
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        printf("Erreur IMG_Init : %s\n", IMG_GetError());
        goto Quit;
    }

    
    
    SDL_Texture *texturePersoPrincipal = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/IDLE.png");
    if(texturePersoPrincipal == NULL){
        printf("Erreur de chargement image pour le perso", SDL_GetError());
        goto Quit;
    }
    

    SDL_Rect perso;

    const int frameWidth = 96;
    const int frameHeight = 84;
    const int idleRow = 0;
    const int idleFrames = 7;
    const int frameTime = 120;

    


    
    SDL_Texture *textureBackground = LotDeTexture(renderer, "assets/background/PNG/City2/Bright/City2.png");

    if (textureBackground == NULL){
        printf("Impossible de charge la texture\n", SDL_GetError());
        goto Quit;
    }

    SDL_Rect rectangle;

    if(SDL_QueryTexture(textureBackground, NULL, NULL, &rectangle.w, &rectangle.h) != 0){
        printf("SDL_Query n'a pas marché\n", SDL_GetError());
        goto Quit;
    }

    rectangle.x = (WIDTH - rectangle.w) / 2;
    rectangle.y = (HEIGHT - rectangle.h) / 2;

    if(SDL_RenderCopy(renderer, textureBackground, NULL, &rectangle) != 0){
        printf("SDL_RendererCopy n'a pas marché\n", SDL_GetError());
        goto Quit;
    }

    SDL_Texture *attackPerso = IMG_LoadTexture(renderer, "assets/tiles/Characters/animation/ATTACK3.png");
    SDL_Texture *idlePerso = IMG_LoadTexture(renderer, "assets/tiles/Characters/animation/IDLE.png");



    SDL_Rect srcPersoPrincipal = {0, idleRow * frameHeight, frameWidth, frameHeight};
    SDL_Rect dstPersoPrincipal = { (WIDTH - frameWidth*4)/2, (HEIGHT - frameHeight*4)/2, frameWidth*4, frameHeight*4 };


    
    int isAttacking = 0;
    Uint32 attackStartTime = 0;
    const Uint32 attackDuration = 100;

    

    SDL_bool program_launched = SDL_TRUE;
    
    
    while(program_launched){
        SDL_Event event;

        while (SDL_PollEvent(&event)){

            switch (event.type){

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){

                        case SDLK_k:

                            isAttacking = 1;
                            attackStartTime = SDL_GetTicks();

                            if(isAttacking != 1){
                                printf("Erreur sur isAttacking", SDL_GetError());
                                goto Quit;
                            }

                        break;

                        default:
                            continue;
                    }


                break;

                
                case SDL_MOUSEBUTTONDOWN:
                        if(event.button.button == SDL_BUTTON_LEFT){
                            printf("Double gauche\n");
                        }
                        if(event.button.button == SDL_BUTTON_RIGHT){
                            printf("Double droit\n");
                        }
                break;
                    

                case SDL_WINDOWEVENT:
                        if(event.window.event == SDL_WINDOWEVENT_LEAVE){
                            printf("La souris est sortie de la fenetre !\n");
                        }

                        if(event.window.event == SDL_WINDOWEVENT_ENTER){
                            printf("La souris est entree dans la fenetre !\n");
                        }
                break;
                    

                case SDL_QUIT:
                        program_launched = SDL_FALSE;
                break;
                
                default:
                break;
            }

        }
        
        frame_limit = SDL_GetTicks() + FPS_LIMIT;

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textureBackground, NULL, NULL);

        if(isAttacking){
            SDL_RenderCopy(renderer, attackPerso, &srcPersoPrincipal, &dstPersoPrincipal);
            if(SDL_GetTicks() - attackStartTime >= attackDuration){
                isAttacking = 0;
            }
        } else {
            SDL_RenderCopy(renderer, idlePerso, &srcPersoPrincipal, &dstPersoPrincipal);
        }


        SDL_RenderPresent(renderer);
        SDL_LimitFPS(frame_limit);
    }
    /*----------------------------------------------------*/


   
    
    

    Quit:
        SDL_DestroyTexture(textureBackground);
        SDL_DestroyTexture(idlePerso);
        SDL_DestroyTexture(attackPerso);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    
    return EXIT_SUCCESS;
}

//Je dois régler d'abord le problème au niveau de la fonction pour IMG_Init !!