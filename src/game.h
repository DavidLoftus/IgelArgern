#include "cell.h"
//struct defining the player
typedef struct player_t
{
    char playerName[32];//player name
    color playerColor;//player color choosen by the player
    token_t tokens[4];// tokens for each player
} player_t;

#define NUM_ROWS 6 //number of rows of the board
#define NUM_COLUMNS 9//number of columns of the board
#define MAX_PLAYERS 6//maximum number of players that can play

typedef struct game_t
{
    cell_t board[NUM_ROWS][NUM_COLUMNS];   // 6 x 9 board
    int numplayers; // number of players
    player_t players[MAX_PLAYERS]; // up to 6 players
} game_t;

void game_init(game_t* game);
void game_run(game_t* game);

void game_drawboard(game_t* game);

void game_move_token_forward(game_t* game, int row, int col);
void game_move_token_up(game_t* game, int row, int col);
void game_move_token_down(game_t* game, int row, int col);
bool game_can_move_token(const game_t* game, int row, int col);
void sidestep_move(game_t* game, int playerId);
