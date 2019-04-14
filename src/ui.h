
#include <curses.h>


void wmsgboxf(WINDOW* stdscr, const char* fmt, ...);
int wpromptf(WINDOW* stdscr, const char* msg, const char* formatString, ...);

#define msgboxf(...) wmsgboxf(stdscr, __VA_ARGS__)
#define promptf(...) wpromptf(stdscr, __VA_ARGS__)
