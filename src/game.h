#include "cell.h"

typedef struct player_t
{
    char* playerName;
    color playerColor;
    token_t tokens[4];
} player_t;

//void player_init(game_t* game, int i);

typedef struct game_t
{
    cell_t board[6][9];   // 6 x 9 board
    int numplayers;
    player_t players[6]; // up to 6 players
} game_t;

void game_init(game_t* game);
