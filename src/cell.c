#include "cell.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

bool enabled;
void cell_init(cell_t* cell)
{
    cell->flags = 0;
    cell->top = NULL;
    cell->height = 0;
}

void cell_finit(cell_t* cell, cellflags flags)
{
    cell->flags = flags;
    cell->top = NULL;
    cell->height = 0;
}
/* push a token into the stack */
void cell_push_token(cell_t* cell, token_t* token)
{
    // Should push tokenColor to top of cell's stack.
    struct stack_node * node = malloc(sizeof(struct stack_node));
    node->token = token;
    node->next = cell->top;
    cell->top = node;

    cell->height++;// Increase element count in stack
}
/* pop a token from the stack */
token_t* cell_pop_token(cell_t* cell)
{
    // Should pop token from top of stack and return the token
    // fails if cell is empty
    struct stack_node* node = cell->top;

    assert(node != NULL);

    token_t* token = node->token;

    cell->top = node->next; // Remove node from list by setting next node to top

    free(node); // Free removed node

    cell->height--;//decrease element count in stack

    return token;
}
bool cell_is_empty(const cell_t* cell)
{
    return cell->top == NULL;
}

/*
Returns the first letter associated with the color of the token
*/
char color_char(color col)
{
    switch(col)
    {
	case RED:
		return 'R';
	case GREEN:
		return 'G';
	case YELLOW:
		return 'Y';
	case BLUE:
		return 'B';
	case MAGENTA:
		return 'M';
	case CYAN:
		return 'C';
    }
}
/*
 * Prints the cell
 *
 * Input: the cell to be printed.
 */
void cell_print(const cell_t* cell)
{
    if(!cell_is_empty(cell))
    {
        token_t* token = cell->top->token;
        char c = color_char(token->tokenColor);
        if( cell->flags & OBSTACLE )
        {
            printf("#%c#", c);
        }
        else
        {
            printf(" %c ", c);
        }
    }
    else if( cell->flags & OBSTACLE )
    {
        printf(" # ");//'#' if the cell represents an obstacle
    }
    else 
    {
        printf("   "); //cell is assigned an empty space otherwise
    }
}

token_t* cell_peek(const cell_t* cell)
{
    assert(!cell_is_empty(cell));
    return cell->top->token;
}
