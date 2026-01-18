#include <stdio.h>
#include "save.h"

int save_game(const char *filename, GameState *state) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Erreur de sauvegarde");
        return 0;
    }
    
    fwrite(state, sizeof(GameState), 1, f);
    fclose(f);
    return 1;
}

int load_game(const char *filename, GameState *state) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        return 0;
    }
    
    fread(state, sizeof(GameState), 1, f);
    fclose(f);
    return 1;
}
