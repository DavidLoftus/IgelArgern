#include <stdbool.h>
//colors of tokens
typedef enum
{
    RED=1,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN
} color;

/* Returns the first letter associated with the color of the token */
char color_char(color col);

//defines a token.
//Note each token can be associated with a color
typedef struct token_t
{
    int teamId;
    color tokenColor;
} token_t;
// A structure to represent a stack
struct stack_node
{
    token_t* token;
    struct stack_node* next;
};

//types of squares
typedef enum
{
    OBSTACLE = 1,
    FINISH = 2
} cellflags;

typedef struct cell_t
{
    cellflags flags;
    int height;// struct member to mark cell height
    struct stack_node* top; // struct member to mark top of stack in array
} cell_t;

void cell_init(cell_t* cell);
void cell_finit(cell_t* cell, cellflags flags);

void cell_print(const cell_t* cell);

bool cell_is_empty(const cell_t* cell);//Functon prototype to f cell is empty
void cell_push_token(cell_t* cell, token_t* token);// Function to add token to stack.
token_t* cell_pop_token(cell_t* cell);// Function prototype to remove token from stack.
token_t* cell_peek(const cell_t* cell);// Function prototype to get top token from stack
