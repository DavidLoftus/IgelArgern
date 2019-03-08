#include "cell.h"

typedef struct player_t
{
    char* playerName;
    color playerColor;
    token_t tokens[4];
} player_t;

void player_init(game_t* game, int i);

typedef struct game_t
{
    cell_t board[6][9];   // 6 x 9 board
    /// TODO: define token_t and uncomment this
    //token_t tokens[6][4]; // up to 6 teams with 4 tokens each
} game_t;
