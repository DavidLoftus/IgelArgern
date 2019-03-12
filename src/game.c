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
