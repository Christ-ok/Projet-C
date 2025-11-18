#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#define WIDTH 255
#define HEIGHT 255


SDL_Texture *LotDeTexture(SDL_Renderer * renderer, const char *path){
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture){
        SDL_Log("Erreur de chargement (%s): %s", path, IMG_GetError);
    }
}


SDL_Texture **loadTabTexture(SDL_Renderer *renderer, char *PathFolder, const char *extension, int taille){
    SDL_Texture **tabPersos = malloc(sizeof(SDL_Texture *) *taille);

    if(!tabPersos){
        perror("Erreur d'allocation sur le tableau de personnages");
        exit(1);
    }
     //C'est ici que la sortie du sprintf sera enregistrée

    for (int i = 0; i < taille; i++){
        char complete_path[300] = "";
        char nbrZeros[3] = "";
        char bufferSprintf[16];
        
        strcpy(complete_path, PathFolder); //mov(complete_path, PathFolder)
        sprintf(bufferSprintf, "%d", i);//

        if (i < 1000){
            strcat(nbrZeros, "0");
        }
        if (i < 100)
        {
            strcat(nbrZeros, "0");
        }
        if (i < 10)
        {
            strcat(nbrZeros, "0");
        }

        strcat(nbrZeros, bufferSprintf);// nbrde0 + i 
        
        strcat(complete_path, nbrZeros);//Construction du chemin jusqu'à tiles du 1er élément + le nombre convertit en chaine de caractère

        strcat(complete_path, extension);

        SDL_Texture *texture = LotDeTexture(renderer, complete_path);//Me permet de récuperer la texture de mon choix
        
        tabPersos[i] = texture; //On met la texture récuperé à l'intérieur du tableau qui contiendra toutes les textures 

        
    }
    
    return tabPersos;
}


int main(int argc, char* argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *surface = NULL;
    SDL_Texture *lotDeTErre = NULL;

    
    if(0 != SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    
    if (NULL == window){
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (NULL == renderer){
        fprintf(stderr, "Erreur SDL_Createrenderer : %s", SDL_GetError());
        goto Quit;
    }
    
    
    if (0 != SDL_RenderClear(renderer))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }

    
    surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
    if (NULL == surface){
        fprintf(stderr, "Erreur SDL_CreateRGBSurface : %s", SDL_GetError());
        goto Quit;
    }

    //Récuperation de la texture
        lotDeTErre = LotDeTexture(renderer, "./assets/tiles/Characters/tile_0000.png");

        //Récuperation de la taille de la texture 
        int largeurLotDeTerre;
        int hauteurLotDeTerre;

        SDL_QueryTexture(lotDeTErre, NULL, NULL, &largeurLotDeTerre, &hauteurLotDeTerre);//FOnctionne comme un scanf
        //

        //Hit Box : la position + la taille de la texture, vérifier les collisions
        SDL_Rect terreRectange = {HEIGHT-50, WIDTH-50 , largeurLotDeTerre, hauteurLotDeTerre};

         SDL_Texture **monPerso = loadTabTexture(renderer, "./assets/tiles/Characters/tile_", ".png", 2);
        

        int largeurPerso;
        int hauteurPerso; 
        
        SDL_QueryTexture(monPerso[0], NULL, NULL, &largeurPerso, &hauteurPerso);

        SDL_Rect persoRectangle = {HEIGHT-100, WIDTH-100 , largeurPerso, hauteurPerso};

        int count = 0;//pour récuperer la premiere valeur du tableau de perso

    int countTurn;

    while (countTurn < 100000)
    {
    
        //Le Staging Area (git), où je vais charger toutes les textures à afficher
        SDL_RenderCopy(renderer, lotDeTErre, NULL, &terreRectange);

        SDL_RenderCopy(renderer, monPerso[count], NULL, &persoRectangle);
        

        if (count == 1){
            count = 0;
        } else{
            count += 1;
        }
        
        //Endroit où j'affiche les images (git commit)
        SDL_RenderPresent(renderer);


        SDL_Delay(150);
        countTurn ++;
    }
    
    
    SDL_FreeSurface(surface);
    
    
    Quit:
    if (NULL != renderer)
    SDL_DestroyRenderer(renderer);
    if (NULL != window)
    SDL_DestroyWindow(window);
      
   SDL_Quit();
   return 0;
}

//les fonctions pour changer les paramètres commencent toutes par SDL_SetWindow ;
//les fonctions pour obtenir les paramètres commencent toutes par SDL_GetWindow ;