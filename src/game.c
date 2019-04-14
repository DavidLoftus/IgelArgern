#include "game.h"
#include "ui.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>

#include <ncurses/curses.h>

const char* readline(char* str, int n)
{
    str = fgets(str, n, stdin);
    if(str)
        str[strcspn(str, "\r\n")] = '\0'; // Finds first occurence of \r or \n and sets it to \0
    return str;
}

char* string_upper(char* str)
{
	for(char* pStr = str; *pStr; ++pStr)
	{
		*pStr = toupper(*pStr);
	}
	return str;
}

//This function creates players for the first time
void init_player(game_t* game, int id)
{
    static const char msgFormat[] = "Player %d, please enter your name.";

    player_t* player = &game->players[id];

    char msg[sizeof(msgFormat)];
    sprintf(msg, msgFormat, id+1);

    promptf(msg, "%s", game->players[id].playerName);

    char color[16];
    bool valid = false;
    while(!valid)
    {
        promptf("Choose your color (RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN)", "%s", color);
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
            msgboxf("Invalid colour, try again.");
            continue;
        }

        for(int i = 0; i < id; ++i)
        {
            if(game->players[i].playerColor == player->playerColor)
            {
                msgboxf("Player %d already has that color, try again.", i+1);
                valid = false;
                break;
            }
        }
    }

    for(int i = 0; i < 4; ++i)
    {
        player->tokens[i].teamId = id;
        player->tokens[i].tokenColor = player->playerColor;
    }
}

//  Place tokens in the first column of the board
void place_token(game_t* game, int playerId, int tokenId)
{
    // Should prompt user for the row they'd like to place their next token on
    // Should then perform checks to see if that location is a valid choice, otherwise loops

    int minheight = INT_MAX;
    for(int i = 0; i < NUM_ROWS; ++i)
    {
        if((cell_is_empty(&game->board[i][0]) || cell_peek(&game->board[i][0])->teamId != playerId) && minheight > game->board[i][0].height)
        {
            minheight = game->board[i][0].height;
        }
    }

    player_t* player = &game->players[playerId];

    printf("%s (%c), which row do you want to place your token (1 - 6): ", player->playerName, color_char(player->playerColor));

    int row;
    while(1)
    {
        if(scanf("%d", &row) != 1 || row < 1 || row > NUM_ROWS)
        {
            printf("Invalid input, try again: ");
            continue;
        }

        row--; // 1-based index to 0-based

        if(game->board[row][0].height > minheight)
        {
            printf("That row is too high, try again: ");
            continue;
        }

        break;
    }

    cell_push_token(&game->board[row][0], &player->tokens[tokenId]);
}
// function defintion for placing the tokens at initial stage
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

/*
 * This function creates the board for the first time
 *
 * Input: board - a 6x9 array of cells
 *
 */
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
    stdscr = initscr();
    noecho();

    game_drawboard(game);

    int res;
    do {
        res = promptf("Enter the number of players.", "%d", &game->numplayers);
    } while(res < 1);

    if(game->numplayers > MAX_PLAYERS)
    {
        msgboxf("Too many players, max is %d.\n", MAX_PLAYERS);
        endwin();
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
    //Check if all 3 tokens of a single color are at the final column
    int complete[MAX_PLAYERS];
    for (size_t i = 0; i < NUM_ROWS; ++i)
    {
        for(struct stack_node* node = game->board[i][NUM_COLUMNS-1].top; node; node = node->next)
        {
            if(++complete[node->token->teamId] > 3)
            {
                if(pWinner)
                    *pWinner = node->token->teamId;
                return true;
            }
        }
    }
    return false;
}


bool ask_sidestep(game_t* game, int row, int col)
{
    char input[32];
    printf("Would you like to move up or down? (press enter to go back) ");
    while(1)
    {
        fgets(input, sizeof(input), stdin);

        if(input[0] == '\0' || (input[0] == '\r' && input[1] == '\n') || input[0] == '\n')
        {
            return false;
        }

        switch(toupper(input[0]))
        {
            case 'U':
                if(row != 0)
                {
                    game_move_token_up(game, row, col);
                    return true;
                }
                printf("Can't move that token up, try again: ");
                break;
            case 'D':
                if(row != NUM_ROWS-1)
                {
                    game_move_token_down(game, row, col);
                    return true;
                }
                printf("Can't move that token down, try again: ");
                break;
            default:
                printf("Invalid input, try again: ");
                break;
        }
    }
}

void sidestep_move(game_t* game, int playerId)
{
    char input[32];
    printf("Please select the row and column of the token you would like to sidestep (or nothing to skip): ");
    while(1)
    {
        fgets(input, sizeof(input), stdin);

        if(input[0] == '\0' || (input[0] == '\r' && input[1] == '\n') || input[0] == '\n')
            break;

        int row,col;
        int count = sscanf(input, "%d %d", &row, &col);
        if ( count != 2 || col > NUM_COLUMNS || row > NUM_ROWS || row < 1 || col < 1 )
        {
            printf("Input is invalid, try again: ");
        }
        else if(cell_is_empty(&game->board[row-1][col-1]))
        {
            printf("The cell is empty, try again: ");
        }
        else if(cell_peek(&game->board[row-1][col-1])->teamId != playerId)
        {
            printf("You can only sidestep your own tokens, try again: ");
        }
        else
        {
            if(ask_sidestep(game, row-1, col-1))
            {
                printf("Sidestep successful!\n");
                game_drawboard(game);
                break;
            }
            else
            {
                printf("Please select the row and column of the token you would like to sidestep (or nothing to skip): ");
            }
        }

    }
}
bool check_move (game_t* game, int row)
{
    //checks the rows for at least one moveable token
    //If no possible move --> skipping turns.
    for (size_t i = 0; i < NUM_COLUMNS; ++i){
        if(!cell_is_empty(&game->board[row][i]) && game_can_move_token(game, row, i))
        {
            return true;
        }
    }
    return false;
}

// This function is responsible for moving the token
void forward_move(game_t* game, int playerId, int row)
{

    if (!check_move(game,row))
    {
        printf("There is no possible move available\n");
        return;
    }
    //prompt asking user whether wants to move token forward
    printf("Pick the column of the token you would like to move forward (along row %d): ", row+1);

    int col;
    while(1)
    {
        int count = scanf("%d", &col);
        if(count == 1 && 1 <= col && col <= NUM_COLUMNS)
        {
            if(!cell_is_empty(&game->board[row][col-1]))
            {
                if(game_can_move_token(game, row, col-1))
                {
                    break;
                }
                else
                {
                    printf("You that token is blocked, try again: ");
                }
            }
            else
            {
                printf("That cell is empty, try again: ");
            }
        }
        else
        {
            printf("Invalid input please enter a number from 1 to %d: ", NUM_COLUMNS);
        }
    }

    game_move_token_forward(game, row, col-1);

}

void game_run(game_t* game)
{
    int winner;
    int playerId = 0;
    while(!check_winner(game, &winner))
    {
        game_drawboard(game);
        player_t* player = &game->players[playerId];

        int dice_roll = rand() % 6+1;

        msgboxf("%s (%c) rolled a %d\n", player->playerName, color_char(player->playerColor), dice_roll);
        sidestep_move(game, playerId);

        forward_move(game, playerId, dice_roll-1);

        playerId = (playerId + 1) % game->numplayers;
    }
    msgboxf("The winner is %s!\n", game->players[playerId].playerName);
    endwin();
}

void game_move_token_forward(game_t* game, int row, int col)
{
    // Assert that there is a cell in front of current cell (simple bounds check)
    // 1. pop token from stack using cell_pop_token (provide pointer to cell e.g. &cell->board[row][col])
    // 2. push that token to the cell in front (board[row][col+1])
    assert(!cell_is_empty(&game->board[row][col]));
    token_t *token = cell_pop_token(&game->board[row][col]);
    cell_push_token(&game->board[row][col+1],token);

}

void game_move_token_up(game_t* game, int row, int col)
{
    // Same as game_move_token_forward but for (row-1,col)
    assert(!cell_is_empty(&game->board[row][col]));
    token_t *token = cell_pop_token(&game->board[row][col]);
    cell_push_token(&game->board[row-1][col],token);
}

void game_move_token_down(game_t* game, int row, int col)
{
    // Same as game_move_token_forward but for (row+1,col)
    assert(!cell_is_empty(&game->board[row][col]));
    token_t *token = cell_pop_token(&game->board[row][col]);
    cell_push_token(&game->board[row+1][col],token);
}

bool game_can_move_token(const game_t* game, int row, int col)
{
    if(game->board[row][col].flags & OBSTACLE)
    {
        // Check all tiles behind this column for tokens.
        // Return false if found otherwise return true
        for(size_t i = 0; i < col; ++i)
        {

            if (!cell_is_empty(&game->board[row][col]))
            {
                return false;
            }

        }
    }
    return true;
}

WINDOW* boardscr = NULL;

void drawcell(game_t* game, int row, int col)
{
    cell_t* cell = &game->board[row][col];
    if(!cell_is_empty(cell))
    {
        waddch(boardscr, 'O');
    }
    else if(cell->flags & OBSTACLE)
    {
        waddch(boardscr, '#');
    }
    else
    {
        waddch(boardscr, ' ');
    }
}

void game_drawboard(game_t* game)
{

    if(boardscr == NULL)
    {
        boardscr = newwin(NUM_ROWS + 2, NUM_COLUMNS + 2, 0, 0);
    }

    wclear(boardscr);
    box(boardscr, 0, 0);

    for(int i = 0; i < NUM_ROWS; i++)
    {
        wmove(boardscr, i+1, 1);
        for(int j = 0; j < NUM_COLUMNS; j++)
        {
            drawcell(game, i, j);
        }
    }

    wrefresh(boardscr);
    wgetch(boardscr);
}
