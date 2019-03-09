#include "cell.h"
#include <stdlib.h>

void cell_init(cell_t* cell)
{
    cell->flags = 0;
    cell->top = NULL;
}

void cell_finit(cell_t* cell, cellflags flags)
{
    cell->flags = flags;
    cell->top = NULL;
}

void cell_push_token(cell_t* cell, token_t* token)
{
    // Should push tokenColor to top of cell's stack.
    struct stack_node * node = malloc(sizeof(struct stack_node));
    if(cell->top == NULL)
    {
        exit(0);
    }
    node->token = token;
    node->next = cell->top;
    cell->top = node;
}

token_t* cell_pop_token(cell_t* cell)
{
    // Should pop token from top of stack and return the token
    // fails if cell is empty
    struct node*  = node ;
    *cell = head->token;
    node = node->next;
    free(cell->top);
    return NULL;
}
bool cell_is_empty(const cell_t* cell)
{
    return cell == NULL ? 1 : 0;
}
