#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#define WIDTH 800
#define HEIGHT 600
#define FPS_LIMIT 16

void SDL_LimitFPS(unsigned int limit)
{

    unsigned int ticks = SDL_GetTicks();

    if (limit < ticks)
    {
        return;
    }
    else if (limit > ticks + FPS_LIMIT)
    {
        SDL_Delay(FPS_LIMIT);
    }
    else
    {
        SDL_Delay(limit - ticks);
    }
}

int main(int argc, char *argv[])
{

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture2 = NULL;
    SDL_Surface *surface = NULL;
    int statut = EXIT_FAILURE;
    unsigned int frame_limit = 0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }

    window = SDL_CreateWindow("BLADE QUEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        fprintf(stderr, "Erreur IMG_Init : %s\n", IMG_GetError());
        goto Quit;
    }

    SDL_Texture *texturePersoPrincipal = IMG_LoadTexture(renderer, "./assets/tiles/Characters/animation/IDLE.png");
    if (texturePersoPrincipal == NULL)
    {
        printf("Erreur de chargement image pour le perso", SDL_GetError());
        goto Quit;
    }

    SDL_Rect perso;

    const int frameWidth = 96;
    const int frameHeight = 84;
    const int idleRow = 0;
    const int idleFrames = 7;
    const int frameTime = 120;

    int currentFrame = 0;
    Uint32 lastFrameTime = 0;

    const int AttackFrames = 6;
    int currentFrameAttack = 0;
    Uint32 lastFrameTimeAttack = 0;

    const int RunFrames = 8;
    int currentFrameRun = 0;
    Uint32 lastFrameTimeRun = SDL_GetTicks();

    SDL_Texture *attackPerso = IMG_LoadTexture(renderer, "assets/tiles/Characters/animation/ATTACK3.png");
    SDL_Texture *idlePerso = IMG_LoadTexture(renderer, "assets/tiles/Characters/animation/IDLE.png");
    SDL_Texture *runPerso = IMG_LoadTexture(renderer, "assets/tiles/Characters/animation/RUN.png");

    int isIdle = 1;
    int isRunning = 0;
    Uint32 runStartTime = 0;

    int isAttacking = 0;
    Uint32 attackStartTime = 0;
    const Uint32 attackDuration = 630;

    SDL_bool program_launched = SDL_TRUE;

    while (program_launched)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {

                case SDLK_k:

                    isIdle = 0;
                    isAttacking = 1;
                    currentFrameAttack = 0;
                    lastFrameTimeAttack = SDL_GetTicks();
                    attackStartTime = SDL_GetTicks();

                    if (isAttacking != 1)
                    {
                        printf("Erreur sur isAttacking", SDL_GetError());
                        goto Quit;
                    }

                    break;

                case SDLK_d:

                    isRunning = 1;
                    runStartTime = SDL_GetTicks();

                    break;

                default:

                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    printf("clic gauche\n");
                }
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    printf("clic droit\n");
                }
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_LEAVE)
                {
                    printf("La souris est sortie de la fenetre !\n");
                }

                if (event.window.event == SDL_WINDOWEVENT_ENTER)
                {
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

        const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

        isRunning = keyboard[SDL_SCANCODE_D] && !isAttacking;
        isIdle = !isRunning && !isAttacking;

        frame_limit = SDL_GetTicks() + FPS_LIMIT;

        SDL_RenderClear(renderer);

        if (isAttacking)
        {
            Uint32 now = SDL_GetTicks();
            if (now - lastFrameTimeAttack >= frameTime)
            {
                currentFrameAttack = (currentFrameAttack + 1) % AttackFrames;
                lastFrameTimeAttack = now;
            }

            SDL_Rect srcPersoPrincipal;
            srcPersoPrincipal.x = currentFrameAttack * frameWidth;
            srcPersoPrincipal.y = idleRow * frameHeight;
            srcPersoPrincipal.w = frameWidth;
            srcPersoPrincipal.h = frameHeight;

            SDL_Rect dstPersoPrincipal;
            dstPersoPrincipal.w = frameWidth * 4;
            dstPersoPrincipal.h = frameHeight * 4;
            dstPersoPrincipal.x = (800 - dstPersoPrincipal.w) / 2;
            dstPersoPrincipal.y = (600 - dstPersoPrincipal.h) / 2;

            SDL_RenderCopy(renderer, attackPerso, &srcPersoPrincipal, &dstPersoPrincipal);
            if (SDL_GetTicks() - attackStartTime >= attackDuration)
            {
                isAttacking = 0;
            }
        }
        else if (isRunning)
        {
            Uint32 now = SDL_GetTicks();
            if (now - lastFrameTimeRun >= frameTime)
            {
                currentFrameRun = (currentFrameRun + 1) % RunFrames;
                lastFrameTimeRun = now;
            }

            SDL_Rect srcPersoPrincipal;
            srcPersoPrincipal.x = currentFrameRun * frameWidth;
            srcPersoPrincipal.y = idleRow * frameHeight;
            srcPersoPrincipal.w = frameWidth;
            srcPersoPrincipal.h = frameHeight;

            SDL_Rect dstPersoPrincipal;
            dstPersoPrincipal.w = frameWidth * 4;
            dstPersoPrincipal.h = frameHeight * 4;
            dstPersoPrincipal.x = (800 - dstPersoPrincipal.w) / 2;
            dstPersoPrincipal.y = (600 - dstPersoPrincipal.h) / 2;

            SDL_RenderCopy(renderer, runPerso, &srcPersoPrincipal, &dstPersoPrincipal);
        }
        else
        {
            Uint32 now = SDL_GetTicks();
            if (now - lastFrameTime >= frameTime)
            {
                currentFrame = (currentFrame + 1) % idleFrames;
                lastFrameTime = now;
            }
            SDL_Rect srcPersoPrincipal;
            srcPersoPrincipal.x = currentFrame * frameWidth;
            srcPersoPrincipal.y = idleRow * frameHeight;
            srcPersoPrincipal.w = frameWidth;
            srcPersoPrincipal.h = frameHeight;

            SDL_Rect dstPersoPrincipal;
            dstPersoPrincipal.w = frameWidth * 4;
            dstPersoPrincipal.h = frameHeight * 4;
            dstPersoPrincipal.x = (800 - dstPersoPrincipal.w) / 2;
            dstPersoPrincipal.y = (600 - dstPersoPrincipal.h) / 2;

            SDL_RenderCopy(renderer, idlePerso, &srcPersoPrincipal, &dstPersoPrincipal);

            lastFrameTimeRun = SDL_GetTicks();
        }

        SDL_RenderPresent(renderer);
        SDL_LimitFPS(frame_limit);
    }

Quit:
    SDL_DestroyTexture(idlePerso);
    SDL_DestroyTexture(attackPerso);
    SDL_DestroyTexture(runPerso);
    SDL_DestroyTexture(texturePersoPrincipal);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
