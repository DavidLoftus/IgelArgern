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

#include <curses.h>

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

const char* choices[] = {
    "RED",
    "GREEN",
    "YELLOW",
    "BLUE",
    "MAGENTA",
    "CYAN"
};

short choiceColors[] = {
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
};

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
        player->playerColor = RED + selectPrompt("Choose your color.", sizeof(choices)/sizeof(choices[0]), choices, choiceColors);
        valid = true;
        for(int i = 0; i < id; ++i)
        {
            if(game->players[i].playerColor == player->playerColor)
            {
                msgboxf("Player %d already has that color.", i+1);
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

    msgboxf("%s (%c), where do you want to place your token.", player->playerName, color_char(player->playerColor));

    int row, col;
    while(1)
    {
        while(!game_select_cell(game, &row, &col));
        if(col != 0)
        {
            msgboxf("Can only select a cell on first column.");
            continue;
        }

        if(game->board[row][0].height > minheight)
        {
            msgboxf("That row is too high, try again.");
            continue;
        }

        if(!cell_is_empty(&game->board[row][0]) && cell_peek(&game->board[row][0])->teamId == playerId)
        {
            msgboxf("Can't block your own token, try again: ");
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
    start_color();
    keypad(stdscr, true);

    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);

    initialize_board(game->board);
    game_drawboard(game);

    while(1)
    {
        int res = promptf("Enter the number of players.", "%d", &game->numplayers);
        if(res == 1)
        {
            if(game->numplayers < 1 || game->numplayers > MAX_PLAYERS)
                msgboxf("Sorry you can only have 1 to %d players.", MAX_PLAYERS);
            else
                break;
        }
    }

    for(int i = 0; i < game->numplayers; ++i)
    {
        init_player(game, i);
    }

    place_tokens(game);

}

bool check_winner(game_t* game, int* pWinner)
{
    //Check if all 3 tokens of a single color are at the final column
    int complete[MAX_PLAYERS] = {0};

    for (size_t i = 0; i < NUM_ROWS; ++i)
    {
        for(struct stack_node* node = game->board[i][NUM_COLUMNS-1].top; node; node = node->next)
        {
            if(++complete[node->token->teamId] >= 3)
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
    static const char* choices[] = {"Up", "Down", "Cancel"};
    int choice = selectPrompt("Would you like to move up or down?", 3, choices, NULL);
    switch(choice)
    {
        case 0:
            if(row != 0)
            {
                game_move_token_up(game, row, col);
                return true;
            }
            msgboxf("Can't move that token up.");
            break;
        case 1:
            if(row != NUM_ROWS-1)
            {
                game_move_token_down(game, row, col);
                return true;
            }
            msgboxf("Can't move that token down.");
            break;
        default:
            break;
    }
    return false;
}

bool check_sidemove (game_t* game, int player)
{
    // Check for atleast one movable token belonging to player
    for (size_t i = 0; i < NUM_ROWS-1; ++i){
        for(size_t j = 0; j < NUM_COLUMNS; ++j)
        {
            if( !cell_is_empty(&game->board[i][j])
             && cell_peek(&game->board[i][j])->teamId == player
             && game_can_move_token(game, i, j))
            {
                return true;
            }
        }
    }
    return false;
}

void sidestep_move(game_t* game, int playerId)
{

    if(!check_sidemove(game, playerId))
    {
        // Skip turn silently if no sidestep available.
        return;
    }

    msgboxf("Please select the token to sidestep (press q to skip).");
    while(1)
    {
        int row,col;
        if(!game_select_cell(game, &row, &col))
        {
            break; // User pressed q to skip
        }
        
        if(cell_is_empty(&game->board[row][col]))
        {
            msgboxf("That cell is empty.");
        }
        else if(cell_peek(&game->board[row][col])->teamId != playerId)
        {
            msgboxf("You can only sidestep your own tokens.");
        }
        else
        {
            if(ask_sidestep(game, row, col))
            {
                game_drawboard(game);
                break;
            }
        }
    }
}
bool check_move (game_t* game, int row)
{
    //checks the rows for at least one moveable token
    //If no possible move --> skipping turns.
    // Dont check NUM_COLUMNS-1 because they can't move forward.
    for (size_t i = 0; i < NUM_COLUMNS-1; ++i){
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
        msgboxf("There is no possible move available, skipping turn.");
        return;
    }

    //prompt message asking user which token to move forward
    msgboxf("Pick the token (along row %d) to move forward.", row+1);

    int _row, col;
    while(1)
    {
        while(!game_select_cell(game, &_row, &col));
        // col can't be NUM_COLUMNS-1 because that is the finish line
        if(_row == row && col != NUM_COLUMNS - 1)
        {
            if(!cell_is_empty(&game->board[row][col]))
            {
                if(game_can_move_token(game, row, col))
                {
                    break;
                }
                else
                {
                    msgboxf("You that token is blocked.");
                }
            }
            else
            {
                msgboxf("That cell is empty.");
            }
        }
        else
        {
            msgboxf("Please select a cell on row %d (not including final column)", row+1);
        }
    }

    game_move_token_forward(game, row, col);

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

        msgboxf("%s (%c) rolled a %d", player->playerName, color_char(player->playerColor), dice_roll);
        sidestep_move(game, playerId);

        forward_move(game, playerId, dice_roll-1);

        playerId = (playerId + 1) % game->numplayers;
    }
    game_drawboard(game);
    msgboxf("The winner is %s!", game->players[playerId].playerName);
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
        for(size_t i = 0; i < NUM_ROWS; ++i)
        {
            for(size_t j = 0; j < col; ++j)
            {
                if (!cell_is_empty(&game->board[i][j]))
                {
                    return false;
                }
            }
        }
    }
    return true;
}
