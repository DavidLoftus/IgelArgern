
#include <curses.h>


void wmsgboxf(WINDOW* stdscr, const char* fmt, ...);
int wpromptf(WINDOW* stdscr, const char* msg, const char* formatString, ...);
int wselectPrompt(WINDOW* stdscr, const char* msg, int nchoices, const char* choices[], short colors[]);

#define msgboxf(...) wmsgboxf(stdscr, __VA_ARGS__)
#define promptf(...) wpromptf(stdscr, __VA_ARGS__)
#define selectPrompt(ms, n, ch, co) wselectPrompt(stdscr, ms, n, ch, co)

typedef struct game_t game_t;

void game_drawboard(const game_t* game);
bool game_select_cell(const game_t* game, int* x, int* y);
