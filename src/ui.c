#include "ui.h"
#include "defs.h"
#include "game.h"
#include <string.h>
#include <stdarg.h>

void wmsgboxf(WINDOW* stdscr, const char* fmt, ...)
{
    // Variadic arguments
    va_list lst;
    va_start(lst, fmt);

    // Query length of resultant message to find required size of window
    int len = vsnprintf(NULL, 0, fmt, lst);
    int h = 5, w = len + 4;

    va_end(lst);

    // Get size of screen
    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);

    WINDOW* win = newwin(h, w, 0, 2*NUM_COLUMNS+3); // Create new window beside board

    box(win, 0, 0); // Surround in box

    wmove(win, 2, 2); // Move to (2,2)

    // Variadic arguments to print message at (2,2)
    va_start(lst, fmt);
    vw_printw(win, fmt, lst);
    va_end(lst);

    wgetch(win); // Wait for user to press a key

    delwin(win); // Delete window

    // Ensure stdscr gets redrawn
    redrawwin(stdscr);
    wrefresh(stdscr);
}

int wpromptf(WINDOW* stdscr, const char* msg, const char* formatString, ...)
{
    // Calculate necessary size of window
    int h = 7, w = strlen(msg) + 4;

    // Fetch size of screen
    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);
    
    
    WINDOW* win = newwin(h, w, 0, 2*NUM_COLUMNS+3); // Create new window beside board

    box(win, 0, 0); // Surround window with box
    
    mvwaddstr(win, 2, 2, msg); // Print message at (2,2)
    

    mvwchgat(win, 4, 2, w-4, A_STANDOUT, 0, NULL); // Set text region to be standout (highlighted)

    // Temporarilly enable echo and STANDOUT for text input. 
    wattron(win, A_STANDOUT); 
    echo();

    // Variadic arguments
    va_list lst;
    va_start(lst, formatString);

    // Move cursor to start of textbox and read in input with given formatString
    wmove(win, 4, 2);
    int resp = vw_scanw(win, formatString, lst);

    va_end(lst);

    // Disable echo and STANDOUT 
    noecho();
    wattroff(win, A_STANDOUT);

    // Delete window after use
    delwin(win);

    // Ensure we redraw stdscr once prompt is closed
    redrawwin(stdscr);
    wrefresh(stdscr);

    return resp;
}

// Draw row of choices onto win, if colors is not null will draw with that color.
void drawChoices(WINDOW* win, int nchoices, const char* choices[], short colors[], int choice)
{
    wmove(win, 4, 2);
    for(int i = 0; i < nchoices; ++i)
    {
        if(colors)
            wcolor_set(win, colors[i], NULL); // Set color if colors is not null
        waddstr(win, "[");

        if(choice == i)
            wattron(win, A_STANDOUT); // If this one is the currently selected, highlight it

        waddstr(win, choices[i]); // print string

        if(choice == i)
            wattroff(win, A_STANDOUT); // Disable highlighting

        waddstr(win, "] ");
    }
    // Set back to default color
    wcolor_set(win, 0, NULL);
}

int wselectPrompt(WINDOW* stdscr, const char* msg, int nchoices, const char* choices[], short colors[])
{
    int len = 0;
    // Calculate necessary length for window
    for(int i = 0; i < nchoices; ++i)
    {
        len += strlen(choices[i]) + 3; // Find sum of lengths of each choice (including 3 characters for formatting)
    }

    // If msg is larger than the choices use it as len
    int msglen = strlen(msg);
    if(msglen > len)
        len = msglen;

    int h = 7, w = len + 4; // Size of window

    // Fetch size of screen
    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);

    WINDOW* win = newwin(h, w, 0, 2*NUM_COLUMNS+3); // Create new window beside board
    keypad(win, true);

    box(win, 0, 0); // Surround window in box

    mvwaddstr(win, 2, 2, msg);

    int choice = 0; // Current choice

    drawChoices(win, nchoices, choices, colors, choice);
    
    // Loop until user presses enter
    for(int c = wgetch(win); c != KEY_ENTER && c != '\n'; c = wgetch(win))
    {
        bool flag = false;
        switch(c)
        {
        case KEY_LEFT:
            // go to previous choice, wraps back to nchoices-1 if it is -1
            choice = (choice + nchoices - 1) % nchoices; // choice - 1 mod nchoices but % doesn't work with negatives
            flag = true;
            break;
        case KEY_RIGHT:
            // go to next choice, wraps back to 0 if it is nchoices
            choice = (choice + 1) % nchoices;
            flag = true;
            break;
        }

        if(flag) // if flag (choice modified) set redraw choices
        {
            drawChoices(win, nchoices, choices, colors, choice);
        }
    }

    // Delete window and redraw stdscr
    delwin(win);

    redrawwin(stdscr);
    wrefresh(stdscr);

    return choice; // return users choice
}

bool game_select_cell(const game_t* game, int* x, int* y)
{
    game_drawboard(game); // Draw board once

    int i = 0, j = 0;
    move(2*i+1, 2*j+1); // Move to cell i,j

    for(int c = getch(); c != KEY_ENTER && c != '\n'; c = getch())
    {
        switch(c)
        {
        case KEY_UP:
            if(i > 0) i--; // Previous row if possible
            move(2*i+1, 2*j+1); // Move to cell i,j
            break;
        case KEY_DOWN:
            if(i < NUM_ROWS-1) i++; // Next row if possible
            move(2*i+1, 2*j+1); // Move to cell i,j
            break;
        case KEY_LEFT:
            if(j > 0) j--; // Previous column if possible
            move(2*i+1, 2*j+1); // Move to cell i,j
            break;
        case KEY_RIGHT:
            if(j < NUM_COLUMNS-1) j++; // Next column if possible
            move(2*i+1, 2*j+1); // Move to cell i,j
            break;
        case 'q':
            return false;
        }
    }

    *x = i;
    *y = j;
    return true;
}

void drawcell(const game_t* game, int row, int col)
{
    const cell_t* cell = &game->board[row][col];

    if(!cell_is_empty(cell))
    {
        color_set(cell_peek(cell)->tokenColor, NULL); // Set colour if not empty
        if(cell->flags & OBSTACLE)
            addch('#'); // Draw obstacle symbol, will be coloured with the color of top token
        else
            addch('O'); // Otherwise draw token symbol
        color_set(0, NULL);
    }
    else if(cell->flags & OBSTACLE)
    {
        addch('#'); // Draw plain obstacle symbol, will have default colour
    }
    else
    {
        addch(' '); // Blank space, draw nothing
    }
}

// Code to draw a line in the form start space div space div space div space ... end
void drawline(chtype start, chtype space, chtype div, chtype end)
{
    addch(start);
    addch(space);
    for(int i = 0; i < NUM_COLUMNS-1; ++i)
    {
        addch(div);
        addch(space);
    }
    addch(end);
}

void game_drawboard(const game_t* game)
{
    clear(); // Clear screen

    move(0, 0); // Move back to start
    drawline(ACS_ULCORNER, ACS_HLINE, ACS_TTEE, ACS_URCORNER); // Draw first line at top

    for(int i = 0; i < NUM_ROWS; i++)
    {
        move(2*i+1, 0); // Move to start of row
        addch(ACS_VLINE); // Draw divider
        for(int j = 0; j < NUM_COLUMNS; j++)
        {
            drawcell(game, i, j);
            addch(ACS_VLINE);
        }
        printw("%d", i+1); // Print current row
        move(2*(i+1), 0);
        if(i != NUM_ROWS-1)
        {
            drawline(ACS_LTEE, ACS_HLINE, ACS_PLUS, ACS_RTEE); // Draw normal line with pluses if not final row
        }
        else
        {
            drawline(ACS_LLCORNER, ACS_HLINE, ACS_BTEE, ACS_LRCORNER); // Draw bottom line if final row
        }
    }
    mvaddstr(2*NUM_ROWS + 1, 0, " 1 2 3 4 5 6 7 8 9"); // Draw column indicators

    refresh(); // Redraw screen
}

