#include "ui.h"

#include <string.h>
#include <stdarg.h>

void wmsgboxf(WINDOW* stdscr, const char* fmt, ...)
{
    va_list lst;

    va_start(lst, fmt);

    int len = vsnprintf(NULL, 0, fmt, lst);
    int h = 5, w = len + 4;

    va_end(lst);

    WINDOW* win = newwin(h, w, 0, 0);

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

    WINDOW* win = newwin(h, w, 0, 0);

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


