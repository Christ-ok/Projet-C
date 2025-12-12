#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

static const char CONFIG_PATH[] = "..\\Projet-C\\config.ini";

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

SDL_Texture *create_text(SDL_Renderer *r, TTF_Font *f, const char *txt, SDL_Color c, int *ow, int *oh)
{
    SDL_Surface *surf = TTF_RenderUTF8_Blended(f, txt, c);
    if (!surf)
        return NULL;
    SDL_Texture *tx = SDL_CreateTextureFromSurface(r, surf);
    if (ow)
        *ow = surf->w;
    if (oh)
        *oh = surf->h;
    SDL_FreeSurface(surf);
    return tx;
}

Menu menu_create(SDL_Renderer *renderer, TTF_Font *font, const char **labels, int count)
{
    Menu m;
    m.count = count;
    m.selected = -1;
    m.items = malloc(sizeof(SDL_Texture *) * count);
    m.rects = malloc(sizeof(SDL_Rect) * count);

    SDL_Color green = {50, 255, 180, 255};
    for (int i = 0; i < count; i++)
    {
        int w, h;
        m.items[i] = create_text(renderer, font, labels[i], green, &w, &h);
        m.rects[i] = (SDL_Rect){0, 0, w, h};
    }
    return m;
}

void menu_free(Menu *m)
{
    for (int i = 0; i < m->count; i++)
        if (m->items[i])
            SDL_DestroyTexture(m->items[i]);
    free(m->items);
    free(m->rects);
}

void menu_layout(Menu *m, int winW, int winH, int startY)
{
    for (int i = 0; i < m->count; i++)
    {
        m->rects[i].x = (winW - m->rects[i].w) / 2;
        m->rects[i].y = startY + i * (m->rects[i].h + 24);
    }
}

void menu_handle_mouse(Menu *m, int mx, int my)
{
    m->selected = -1;
    for (int i = 0; i < m->count; i++)
    {
        SDL_Rect r = m->rects[i];
        if (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h)
            m->selected = i;
    }
}

void menu_render(SDL_Renderer *renderer, Menu *m)
{
    for (int i = 0; i < m->count; i++)
    {
        if (i == m->selected)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 170, 70);
            SDL_Rect r = m->rects[i];
            r.x -= 12;
            r.y -= 8;
            r.w += 24;
            r.h += 16;
            SDL_RenderFillRect(renderer, &r);
        }
        SDL_RenderCopy(renderer, m->items[i], NULL, &m->rects[i]);
    }
}

void save_config(int vol, int w, int h)
{
    FILE *f = fopen(CONFIG_PATH, "w");
    if (!f)
        return;
    fprintf(f, "volume=%d\nwidth=%d\nheight=%d\n", vol, w, h);
    fclose(f);
}

void load_config(int *vol, int *w, int *h)
{
    *vol = 33;
    *w = DEFAULT_WIDTH;
    *h = DEFAULT_HEIGHT;

    FILE *f = fopen(CONFIG_PATH, "r");
    if (!f)
        return;

    char key[64], val[64];
    while (fscanf(f, "%63[^=]=%63s\n", key, val) == 2)
    {
        if (strcmp(key, "volume") == 0)
            *vol = atoi(val);
        else if (strcmp(key, "width") == 0)
            *w = atoi(val);
        else if (strcmp(key, "height") == 0)
            *h = atoi(val);
    }
    fclose(f);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    int vol, winW, winH;
    load_config(&vol, &winW, &winH);

    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

    Mix_Music *bgm = Mix_LoadMUS("..\\Projet-C\\assets\\audio\\menu_bgg.ogg");
    if (bgm)
    {
        Mix_VolumeMusic((vol * MIX_MAX_VOLUME) / 100);
        Mix_PlayMusic(bgm, -1);
    }

    SDL_Window *window = SDL_CreateWindow("Blade Quest",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          winW, winH, SDL_WINDOW_RESIZABLE);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Font *font = TTF_OpenFont("..\\Projet-C\\assets\\fonts\\Titre_menu.ttf", 64);
    SDL_Texture *bg = IMG_LoadTexture(renderer, "..\\Projet-C\\assets\\background\\image_fondv1.png");

    // DEBUG: afficher la taille réelle de la texture chargée
    if (bg)
    {
        int tw = 0, th = 0;
        SDL_QueryTexture(bg, NULL, NULL, &tw, &th);
        printf("DEBUG: bg texture size = %d x %d\n", tw, th);
    }

    const char *mainItems[] = {"JOUER", "PARAMETRES", "QUITTER"};
    const char *jouerItems[] = {"NOUVELLE PARTIE", "CHARGER PARTIE", "RETOUR"};
    const char *paramItems[] = {"AUDIO", "RESOLUTION", "RETOUR"};
    const char *resItems[] = {"1280x720", "1600x900", "1920x1080", "800x600", "RETOUR"};

    Menu mainMenu = menu_create(renderer, font, mainItems, 3);
    Menu jouerMenu = menu_create(renderer, font, jouerItems, 3);
    Menu paramMenu = menu_create(renderer, font, paramItems, 3);
    Menu resMenu = menu_create(renderer, font, resItems, 5);

    char volText[64];
    sprintf(volText, "VOLUME : %d%%", vol);
    const char *audioItems[] = {volText, "RETOUR"};
    Menu audioMenu = menu_create(renderer, font, audioItems, 2);

    MenuState state = MENU_MAIN;
    Menu currentMenu = mainMenu;

    int preset[4][2] = {{1280, 720}, {1600, 900}, {1920, 1080}, {800, 600}};

    int running = 1;
    SDL_Event e;

    while (running)
    {

        SDL_GetWindowSize(window, &winW, &winH);
        menu_layout(&currentMenu, winW, winH, winH * 0.35);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = 0;

            else if (e.type == SDL_MOUSEMOTION)
                menu_handle_mouse(&currentMenu, e.motion.x, e.motion.y);

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                int s = currentMenu.selected;

                if (state == MENU_MAIN)
                {
                    if (s == 0)
                    {
                        state = MENU_JOUER;
                        currentMenu = jouerMenu;
                    }
                    if (s == 1)
                    {
                        state = MENU_PARAM;
                        currentMenu = paramMenu;
                    }
                    if (s == 2)
                        running = 0;
                }

                else if (state == MENU_JOUER && s == 2)
                {
                    state = MENU_MAIN;
                    currentMenu = mainMenu;
                }

                else if (state == MENU_PARAM)
                {
                    if (s == 0)
                    {
                        state = MENU_AUDIO;
                        currentMenu = audioMenu;
                    }
                    if (s == 1)
                    {
                        state = MENU_RESOL;
                        currentMenu = resMenu;
                    }
                    if (s == 2)
                    {
                        state = MENU_MAIN;
                        currentMenu = mainMenu;
                    }
                }

                else if (state == MENU_RESOL)
                {
                    if (s < 4)
                    {
                        SDL_SetWindowSize(window, preset[s][0], preset[s][1]);
                        save_config(vol, preset[s][0], preset[s][1]);
                    }
                    else
                    {
                        state = MENU_PARAM;
                        currentMenu = paramMenu;
                    }
                }

                else if (state == MENU_AUDIO && s == 1)
                {
                    state = MENU_PARAM;
                    currentMenu = paramMenu;
                }
            }

            else if (e.type == SDL_KEYDOWN && state == MENU_AUDIO)
            {
                if (e.key.keysym.sym == SDLK_LEFT)
                    vol = (vol > 0) ? vol - 5 : 0;
                if (e.key.keysym.sym == SDLK_RIGHT)
                    vol = (vol < 100) ? vol + 5 : 100;

                Mix_VolumeMusic((vol * MIX_MAX_VOLUME) / 100);

                sprintf(volText, "VOLUME : %d%%", vol);
                SDL_DestroyTexture(audioMenu.items[0]);

                int w, h;
                audioMenu.items[0] = create_text(renderer, font, volText,
                                                 (SDL_Color){50, 255, 180, 255}, &w, &h);
                audioMenu.rects[0].w = w;
                audioMenu.rects[0].h = h;

                save_config(vol, winW, winH);
            }
        }

        SDL_RenderClear(renderer);
        if (bg)
        {
            SDL_Rect dst = {0, 0, winW, winH};
            SDL_RenderCopy(renderer, bg, NULL, &dst);
        }
        menu_render(renderer, &currentMenu);
        SDL_RenderPresent(renderer);
    }

    menu_free(&mainMenu);
    menu_free(&jouerMenu);
    menu_free(&paramMenu);
    menu_free(&resMenu);
    menu_free(&audioMenu);

    if (bgm)
        Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    Mix_Quit();

    if (bg)
        SDL_DestroyTexture(bg);
    if (font)
        TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
