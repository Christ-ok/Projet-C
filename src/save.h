#ifndef SAVE_H
#define SAVE_H

typedef struct {
    int player_x;
    int player_y;
    int player_hp;
    int camera_x;
} GameState;

int save_game(const char *filename, GameState *state);


int load_game(const char *filename, GameState *state);

#endif
