#include <stdio.h>
#include "save.h"

int save_game(const char *filename, GameState *state) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Erreur");
        return 0;
    }
    
    size_t written = fwrite(state, sizeof(GameState), 1, f);
    fclose(f);
    
    return (written == 1);
}

int load_game(const char *filename, GameState *state) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        return 0;
    }
    
    size_t read = fread(state, sizeof(GameState), 1, f);
    fclose(f);
    
    return (read == 1);
}
