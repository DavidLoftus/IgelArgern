#include "game.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

const char* readline(char* str, int n)
{
    str = fgets(str, n, stdin);
    if(str)
        str[strcspn(str, "\r\n")] = '\0'; // Finds first occurence of \r or \n and sets it to \0
    return str;
}

void skipline()
{
    while(getchar() != '\n' && !feof(stdin));
}

char* string_upper(char* str)
{
	for(char* pStr = str; *pStr; ++pStr)
	{
		*pStr = toupper(*pStr);
	}
	return str;
}

void init_player(game_t* game, int id)
{

    player_t* player = &game->players[id];

    printf("Player %d, enter your name: ", id+1);

    readline(game->players[id].playerName, sizeof(player->playerName));

    printf("Choose your color (RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN): ");
    char color[16];
    bool valid = false;
    for(;;)
    {
        readline(color, sizeof(color));
        string_upper(color);

        if(strcmp(color, "RED") == 0)
        {
            player->playerColor = RED;
            valid = true;
        }
        else if(strcmp(color, "GREEN") == 0)
        {
            player->playerColor = GREEN;
            valid = true;
        }
        else if(strcmp(color, "YELLOW") == 0)
        {
            player->playerColor = YELLOW;
            valid = true;
        }
        else if(strcmp(color, "BLUE") == 0)
        {
            player->playerColor = BLUE;
            valid = true;
        }
        else if(strcmp(color, "MAGENTA") == 0)
        {
            player->playerColor = MAGENTA;
            valid = true;
        }
        else if(strcmp(color, "CYAN") == 0)
        {
            player->playerColor = CYAN;
            valid = true;
        }

        if(!valid)
        {
            printf("Invalid colour, try again: ");
            continue;
        }

        for(int i = 0; i < id; ++i)
        {
            if(game->players[i].playerColor == player->playerColor)
            {
                printf("Player %d already has that color, try again: ", i+1);
                valid = false;
                break;
            }
        }

        if(valid)
        {
            break;
        }
    }

    for(int i = 0; i < 4; ++i)
    {
        player->tokens[i].teamId = id;
        player->tokens[i].tokenColor = player->playerColor;
    }
}

// Places a token
void place_token(game_t* game, int playerId, int tokenId)
{
    // Should prompt user for the row they'd like to place their next token on
    // Should then perform checks to see if that location is a valid choice, otherwise loops

    int minheight = INT_MAX;
    for(int i = 0; i < NUM_ROWS; ++i)
    {
        if((cell_is_empty(&game->board[i][0]) || game->board[i][0].top->token->teamId != playerId) && minheight > game->board[i][0].height)
        {
            minheight = game->board[i][0].height;
        }
    }

    player_t* player = &game->players[playerId];

    printf("%s, which row do you want to place your token (1 - 6): ", player->playerName);

    int row;
    for(;;)
    {
        if(scanf("%d", &row) != 1)
            continue;

        if(row < 1 || row > NUM_ROWS)
        {
            printf("Invalid row, try again: ");
            continue;
        }

        row--; // 1-based index to 0-based

        if(!cell_is_empty(&game->board[row][0]) && game->board[row][0].top->token->teamId == playerId)
        {
            printf("You can't block yourself, try again: ");
            continue;
        }

        if(game->board[row][0].height > minheight)
        {
            printf("That row is too high, try again: ");
            continue;
        }

        break;
    }

    cell_push_token(&game->board[row][0], &player->tokens[tokenId]);
}

void place_tokens(game_t* game)
{
    for(int tokenId = 0; tokenId < 4; ++tokenId)
    {
        for(int playerId = 0; playerId < game->numplayers; playerId++)
        {
            game_drawboard(game);
            place_token(game, playerId, tokenId);
        }
    }
}

void initialize_board(cell_t board[NUM_ROWS][NUM_COLUMNS])
{
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for(int j = 0; j < NUM_COLUMNS ; j++)
        {
            //creates an obstacle square at positions (0,3), (1,6), (2,4), (3,5), (4,2) and (5,7)
            if( (i == 0 && j == 3) || (i == 1 && j == 6) || (i == 2 && j == 4)
             || (i == 3 && j == 5) || (i == 4 && j == 2) || (i == 5 && j == 7) )
            {
                cell_finit(&board[i][j], OBSTACLE);
            }
            else
            {
                cell_init(&board[i][j]);
            }
        }
    }
}

void game_init(game_t* game)
{
    printf("Enter the number of players: ");
    scanf("%d", &game->numplayers);
    skipline();

    if(game->numplayers > MAX_PLAYERS)
    {
        printf("Too many players, max is %d.\n", MAX_PLAYERS);
        exit(1);
    }

    for(int i = 0; i < game->numplayers; ++i)
    {
        init_player(game, i);
    }

    initialize_board(game->board);
    place_tokens(game);
}

bool check_winner(game_t* game, int* pWinner)
{
    // TODO: Check if all 4 tokens of a single color are at the final column
    int complete[MAX_PLAYERS];
    size_t i;
    for (i= 0; i < NUM_ROWS; ++i)
    {
        for(struct stack_node* node = game->board[i][NUM_COLUMNS-1].top; node; node = node->next)
        {
            if(++complete[node->token->teamId] > 4)
            {
                if(pWinner)
                    *pWinner = node->token->teamId;
                return true;
            }

        }
    }
    return false;
}


void game_run(game_t* game)
{
    int winner;
    int player = 0;
    while(!check_winner(game, &winner))
    {
        game_drawboard(game);

        // TODO: Logic for a single move

        player = (player + 1) % game->numplayers; // Next player (loops back when overflows)

        break; // TODO: remove this when game wont just infinite loop
    }
}

void game_move_token_forward(game_t* game, int row, int col)
{
    // TODO: implement
    // Assert there is a token at game->board[row][col] (this won't be tested in release build if we write our code correctly)
    // Assert that there is a cell in front of current cell (simple bounds check)
    // 1. pop token from stack using cell_pop_token (provide pointer to cell e.g. &cell->board[row][col])
    // 2. push that token to the cell in front (board[row][col+1])

    token_t *token = cell_pop_token(&game->board[row][col]);
    cell_push_token(&game->board[row][col+1],token);

}

void game_move_token_up(game_t* game, int row, int col)
{
    // Same as game_move_token_forward but for (row-1,col)
    token_t *token = cell_pop_token(&game->board[row][col]);
    cell_push_token(&game->board[row-1][col],token);
}

void game_move_token_down(game_t* game, int row, int col)
{
    // Same as game_move_token_forward but for (row+1,col)
    token_t *token = cell_pop_token(&game->board[row][col]);
    cell_push_token(&game->board[row+1][col],token);
}

void game_drawboard(game_t* game)
{
    printf("/---------\\\n");
    for(int i = 0; i < NUM_ROWS; i++)
    {
        printf("|");
        for(int j = 0; j < NUM_COLUMNS; j++)
        {
            cell_print(&game->board[i][j]);
        }
        printf("|\n");
    }
    printf("\\---------/\n");
}
