
#include <curses.h>


void wmsgboxf(WINDOW* stdscr, const char* fmt, ...);
int wpromptf(WINDOW* stdscr, const char* msg, const char* formatString, ...);

#define msgboxf(...) wmsgboxf(stdscr, __VA_ARGS__)
#define promptf(...) wpromptf(stdscr, __VA_ARGS__)

typedef struct game_t game_t;

void game_drawboard(const game_t* game);
bool game_select_cell(const game_t* game, int* x, int* y);
