#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>

#include "background.h"
#include "animation.h"
#include "stat.h"
#include "demon.h"
#include "deplacement2.h"
#include "blocs.h"
#include "pause.h"
#include "gameover.h"
#include "save.h"

Personnage perso;

#define WIDTH 800
#define HEIGHT 600
#define NB_BLOCS 10
#define MAX_DEMONS 10

int main(int argc, char *argv[])
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        return -1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        SDL_Log("IMG_Init: %s", IMG_GetError());
        return -1;
    }
    if (TTF_Init() == -1)
    {
        SDL_Log("TTF_Init: %s", TTF_GetError());
        return -1;
    }
    if ((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) != MIX_INIT_OGG)
    {
        SDL_Log("Mix_Init: %s", Mix_GetError());
        return -1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("Mix_OpenAudio: %s", Mix_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("BLADE QUEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    initCharacter(renderer, &perso);
    initImageDemon(renderer);
    initDemon(renderer);
    initBackground(renderer);

    Bloc surface;
    initBloc(renderer, &surface);

    Bloc tab[NB_BLOCS];
    initBlocArray(tab, NB_BLOCS, surface.w, surface.h);

    Bloc level[200];
    generateLevel(level, 200, surface.w, surface.h);

    SDL_bool running = SDL_TRUE;
    int camera_x = 0;
    int camera_y = 0;
    int gauche = 0;
    int state = 0;

    int d_pressed = 0;
    int q_pressed = 0;
    int space_pressed = 0;

    int demonCount = 0;

    int jump_offset = 0;
    int jump_direction = 0;
    const int jump_height = 160;
    const int jump_speed = 7;

    const int speed = 5;
    int PlayerIsLeft = 0;

    int Delay = 650;
    int LastAttackTime = 0;

    Demon horde[MAX_DEMONS];
    Demon_spawn(&horde[0], 1450, 400);
    Demon_spawn(&horde[1], 1000, 200);
    Demon_spawn(&horde[2], 1200, 200);
    Demon_spawn(&horde[3], 2000, 200);
    Demon_spawn(&horde[4], 1200, 400);
    Demon_spawn(&horde[5], 1600, 200);
    Demon_spawn(&horde[6], 2100, 400);
    Demon_spawn(&horde[7], 2700, 400);
    Demon_spawn(&horde[8], 1800, 200);
    Demon_spawn(&horde[9], 2200, 200);

    GameState game_state;

    if (argc > 1 && strcmp(argv[1], "load") == 0)
    {
        if (load_game("savegame.dat", &game_state))
        {
            perso.x = game_state.player_x;
            perso.y = game_state.player_y;
            perso.HP = game_state.player_hp;
            camera_x = game_state.camera_x;
            printf("Partie chargée depuis le menu!\n");
        }
    }

    while (running)
    {
        SDL_Event event;
        Uint32 CurrentTime = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;

            if (event.type == SDL_MOUSEBUTTONDOWN && isGamePaused())
            {
                PauseButtons pauseButtons = {{WIDTH / 2 - 80, HEIGHT / 2 - 20, 160, 40},
                                             {WIDTH / 2 - 80, HEIGHT / 2 + 40, 160, 40}};
                int buttonClicked = handlePauseButtons(event.button.x, event.button.y, &pauseButtons);

                if (buttonClicked == 1)
                {
                    game_state.player_x = perso.x;
                    game_state.player_y = perso.y;
                    game_state.player_hp = perso.HP;
                    game_state.camera_x = camera_x;
                    save_game("savegame.dat", &game_state);
                    printf("Partie sauvegardée depuis le pause!\n");
                }
                else if (buttonClicked == 2)
                {
                    running = SDL_FALSE;
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && perso.alive == 0)
            {
                if (handleGameOverButtons(event.button.x, event.button.y, WIDTH, HEIGHT))
                {
                    running = SDL_FALSE;
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && demonCount == 10)
            {
                if (handleGameOverButtons(event.button.x, event.button.y, WIDTH, HEIGHT))
                {
                    running = SDL_FALSE;
                }
            }

            if (event.type == SDL_KEYDOWN && perso.alive == 1)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    togglePause(renderer, WIDTH, HEIGHT, camera_x, camera_y, jump_offset, state, gauche, PlayerIsLeft, &perso, horde);
                }
                if (event.key.keysym.sym == SDLK_d)
                {
                    d_pressed = 1;
                    state = 1;
                    gauche = 0;
                    perso.state = 1;
                    perso.direction = 0;
                }

                if (event.key.keysym.sym == SDLK_q)
                {
                    q_pressed = 1;
                    state = 1;
                    gauche = 1;
                    perso.state = 1;
                    perso.direction = 1;
                }

                if (event.key.keysym.sym == SDLK_k)
                {
                    if (CurrentTime > LastAttackTime + Delay)
                    {

                        state = 2;
                        perso.state = 2;

                        float same_y = perso.base_y + 90;

                        for (int i = 0; i < MAX_DEMONS; i++)
                        {

                            if (perso.direction == 0 && horde[i].x_base - perso.x <= 270 && horde[i].x_base - perso.x >= 150 && same_y <= horde[i].y_base + 5 && same_y >= horde[i].y_base - 5)
                            {
                                Demon_takeDamage(&horde[i], 1);
                            }

                            if (perso.direction == 1 && horde[i].x_base - perso.x >= 20 && horde[i].x_base - perso.x <= 150 && same_y <= horde[i].y_base + 5 && same_y >= horde[i].y_base - 5)
                            {
                                Demon_takeDamage(&horde[i], 1);
                            }
                        }

                        LastAttackTime = CurrentTime;
                    }
                }

                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    space_pressed = 1;
                    perso.state = 3;
                    jump_direction = 1;
                }

                if (event.key.keysym.sym == SDLK_t)
                {
                    Hero_takeDamage(&perso, 1);
                }

                if (event.key.keysym.sym == SDLK_s)
                {
                    game_state.player_x = perso.x;
                    game_state.player_y = perso.y;
                    game_state.player_hp = perso.HP;
                    game_state.camera_x = camera_x;

                    save_game("savegame.dat", &game_state);
                }
            }

            if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_d)
                {
                    d_pressed = 0;
                    perso.state = 0;
                }

                if (event.key.keysym.sym == SDLK_k)
                {
                    perso.state = 0;
                }

                if (event.key.keysym.sym == SDLK_q)
                {
                    perso.state = 0;
                    q_pressed = 0;
                }

                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    perso.state = 0;
                    space_pressed = 0;
                }

                if (event.key.keysym.sym == SDLK_t)
                {
                    perso.state = 0;
                }
            }
        }
        if (!isGamePaused())
        {
            updateCharacter(&perso, d_pressed, q_pressed, space_pressed, camera_x);

            for (int i = 0; i < MAX_DEMONS; i++)
            {
                updateDemon(&horde[i], d_pressed, q_pressed, space_pressed, &camera_x, &camera_y, &perso);

                float same_y2 = perso.base_y + 90;

                if (horde[i].x_base - perso.x == 150 && horde[i].exists == 1 && same_y2 <= horde[i].y_base + 5 && same_y2 >= horde[i].y_base - 5)
                {
                    Hero_takeDamage(&perso, 1);
                }
            }

            applyGravity(&perso);

            checkCollisionWithBlocs(level, 200, &perso, jump_offset, &jump_direction);

            jumpY(space_pressed, &jump_direction, &jump_offset, jump_height, jump_speed, perso.on_ground);

            cameraY(&camera_x, &camera_y, &perso, jump_offset, WIDTH);
        }

        SDL_RenderClear(renderer);
        if (isGamePaused())
        {
            renderPauseMenu(renderer, WIDTH, HEIGHT);
        }
        else
        {
            drawBackground(renderer, camera_x, camera_y);
            drawBloc(renderer, level, 200, camera_x);
            drawDemonStat(renderer);
            drawCoeurStat(renderer);
            drawCharacter(renderer, &perso, camera_x, camera_y, jump_offset);
            stat(renderer, demonCount);
            statCoeur(renderer, &perso);

            for (int i = 0; i < MAX_DEMONS; i++)
            {
                if (horde[i].exists)
                {
                    drawDemon(renderer, &horde[i], horde[i].currentState);
                }
                if (!horde[i].wascounted && !horde[i].exists)
                {
                    demonCount++;
                    horde[i].wascounted = 1;
                }

                if (perso.alive == 0)
                {
                    GameOver(renderer, WIDTH, HEIGHT);
                }
                if (demonCount == 10)
                {
                    GameFinished(renderer, WIDTH, HEIGHT);
                }
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    cleanupPause();
    cleanupGameOver();
    cleanUpImageDemon();
    cleanupBackground();
    cleanupCharacter();
    cleanupDemon();
    cleanUpImageHP();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();

    return 0;
}

// Code pour lancer le projet :
//gcc src/main.c src/background.c src/animation.c src/demon.c src/stat.c src/deplacement.c src/deplacement2.c src/pause.c src/save.c src/gameover.c src/blocs.c -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o game.exe ; gcc src/menu_opti.c -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o menu.exe