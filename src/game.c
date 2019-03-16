#include "game.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

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
}

void place_token(game_t* game, int id)
{
    // Should prompt user for the row they'd like to place their next token on
    // Should then perform checks to see if that location is a valid choice, otherwise loops
   token_t token = &game->tokens[id];

    printf("Token %s, Place your token: ", id+1);

    readline(game->tokens[id].tokenColor, sizeof(token->tokenColor));
}

void place_tokens(game_t* game)
{
    for(int i = 0; i < 4; ++i)
    {
        for(int id = 0; id < game->numplayers; id++)
        {
            place_token(game, id);
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
}

void game_drawboard(game_t* game)
{
    printf("/-----------\\\n");
    for(int i = 0; i < NUM_ROWS; i++)
    {
        printf("| ");
        for(int j = 0; j < NUM_COLUMNS; j++)
        {
            cell_print(&game->board[i][j]);
        }
        printf(" |\n");
    }
    printf("\\-----------/\n");
}
