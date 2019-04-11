
#include <ncurses/curses.h>


void wmsgboxf(WINDOW* stdscr, const char* fmt, ...);
int wpromptf(WINDOW* stdscr, const char* msg, const char* formatString, ...);

#define msgboxf(msg, ...) wmsgboxf(stdscr, msg, __VA_ARGS__)
#define promptf(msg, fmt, ...) wpromptf(stdscr, msg, fmt, __VA_ARGS__)
