#include "ui.h"
#include "defs.h"
#include "game.h"
#include <string.h>
#include <stdarg.h>

void wmsgboxf(WINDOW* stdscr, const char* fmt, ...)
{
    va_list lst;

    va_start(lst, fmt);

    int len = vsnprintf(NULL, 0, fmt, lst);
    int h = 5, w = len + 4;

    va_end(lst);

    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);

    WINDOW* win = newwin(h, w, (maxy-h)/2, (maxx - w)/2);

    box(win, 0, 0);

    wmove(win, 2, 2);

    va_start(lst, fmt);
    vw_printw(win, fmt, lst);
    va_end(lst);

    wgetch(win);

    delwin(win);

    redrawwin(stdscr);
    wrefresh(stdscr);
}

int wpromptf(WINDOW* stdscr, const char* msg, const char* formatString, ...)
{
    int h = 7, w = strlen(msg) + 4;

    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);

    WINDOW* win = newwin(h, w, (maxy-h)/2, (maxx - w)/2);

    box(win, 0, 0);

    mvwaddstr(win, 2, 2, msg);

    mvwchgat(win, 4, 2, w-4, A_STANDOUT, 0, NULL);

    wattron(win, A_STANDOUT);
    echo();
    wmove(win, 4, 2);

    va_list lst;

    va_start(lst, formatString);

    int resp = vw_scanw(win, formatString, lst);

    va_end(lst);

    noecho();
    wattroff(win, A_STANDOUT);

    delwin(win);

    redrawwin(stdscr);
    wrefresh(stdscr);

    return resp;
}

int wselectPrompt(WINDOW* stdscr, const char* msg, int nchoices, const char* choices[], short colors[])
{
    int len = 0;

    for(int i = 0; i < nchoices; ++i)
    {
        len += strlen(choices[i]) + 3;
    }

    int h = 5, w = len + 4;

    int maxx = getmaxx(stdscr);
    int maxy = getmaxy(stdscr);

    WINDOW* win = newwin(h, w, (maxy-h)/2, (maxx - w)/2);

    box(win, 0, 0);

    wmove(win, 2, 2);

    int choice = 0;

    for(int i = 0; i < nchoices; ++i)
    {
        if(choice == i)
        {
            wattron(win, A_STANDOUT);
        }
        wcolor_set(win, colors[i], NULL);
        wprintw(win, "[%s] ", choices[i]);

        if(choice == i)
        {
            wattroff(win, A_STANDOUT);
        }
    }
    wcolor_set(win, 0, NULL);
    
    for(int c = wgetch(stdscr); c != KEY_ENTER && c != '\n'; c = wgetch(stdscr))
    {
        
    }

    delwin(win);

    redrawwin(stdscr);
    wrefresh(stdscr);

    return choice;
}

WINDOW* boardscr = NULL;

bool game_select_cell(const game_t* game, int* x, int* y)
{
    game_drawboard(game);

    int i = 0, j = 0;
    wmove(boardscr, i+1, j+1);

    for(int c = wgetch(boardscr); c != KEY_ENTER && c != '\n'; c = wgetch(boardscr))
    {
        switch(c)
        {
        case KEY_UP:
            if(i > 0) i--;
            wmove(boardscr, i+1, j+1);
            break;
        case KEY_DOWN:
            if(i < NUM_ROWS-1) i++;
            wmove(boardscr, i+1, j+1);
            break;
        case KEY_LEFT:
            if(j > 0) j--;
            wmove(boardscr, i+1, j+1);
            break;
        case KEY_RIGHT:
            if(j < NUM_COLUMNS-1) j++;
            wmove(boardscr, i+1, j+1);
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
        wcolor_set(boardscr, cell_peek(cell)->tokenColor, NULL);
        waddch(boardscr, 'O');
        wcolor_set(boardscr, 0, NULL);
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

void game_drawboard(const game_t* game)
{

    if(boardscr == NULL)
    {
        boardscr = newwin(NUM_ROWS + 2, NUM_COLUMNS + 2, 0, 0);
        keypad(boardscr, 1);
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
}

