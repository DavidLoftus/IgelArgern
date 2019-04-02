#include "cell.h"

typedef enum
{
   PLACEMENT = 0,
   PLAY = 1,
   GAMEOVER = 2
}stages;

typedef struct player_t
{
    char playerName[32];
    color playerColor;
    token_t tokens[4];
} player_t;

#define NUM_ROWS 6
#define NUM_COLUMNS 9
#define MAX_PLAYERS 6

typedef struct game_t
{
    cell_t board[NUM_ROWS][NUM_COLUMNS];   // 6 x 9 board
    int numplayers;
    player_t players[MAX_PLAYERS]; // up to 6 players
} game_t;

void game_init(game_t* game);
void game_run(game_t* game);

void game_drawboard(game_t* game);
void game_move_token_forward(game_t* game, int row, int col);
void game_move_token_up(game_t* game, int row, int col);
void game_move_token_down(game_t* game, int row, int col);
void sidestep_move(game_t* game, int playerId);
