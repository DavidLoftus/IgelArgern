#include <stdbool.h>

typedef enum
{
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN
} color;

typedef struct token_t
{
    int teamId;
    color tokenColor;
} token_t;

struct stack_node
{
    token_t* token;
    struct stack_node* next;
};

typedef enum
{
    OBSTACLE = 1,
    FINISH = 2
} cellflags;

typedef struct cell_t
{
    cellflags flags;
    int height;
    struct stack_node* top;
} cell_t;

void cell_init(cell_t* cell);
void cell_finit(cell_t* cell, cellflags flags);

void cell_print(const cell_t* cell);

bool cell_is_empty(const cell_t* cell);
void cell_push_token(cell_t* cell, token_t* token);
token_t* cell_pop_token(cell_t* cell);
