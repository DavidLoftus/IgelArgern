#include "cell.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

bool enabled;
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
    node->token = token;
    node->next = cell->top;
    cell->top = node;
}

token_t* cell_pop_token(cell_t* cell)
{
    // Should pop token from top of stack and return the token
    // fails if cell is empty
    struct stack_node* node = cell->top;

    assert(node != NULL);

    token_t* token = node->token;

    cell->top = node->next; // Remove node from list by setting next node to top

    free(node); // Free removed node

    return token;
}
bool cell_is_empty(const cell_t* cell)
{
    return cell->top == NULL;
}

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

void cell_print(const cell_t* cell)
{
    if( cell->flags & OBSTACLE /* && obstacle_is_blocking(cell) */ ) // TODO: only display obstacle char if it is still blocking movement
    {
        putchar('#');
    }
    else if(cell_is_empty(cell))
    {
        putchar(' ');
    }
    else
    {
        token_t* token = cell->top->token;
        putchar(color_char(token->tokenColor));
    }
}
void setEnabled(bool enabledOption) {
		enabled = enabledOption;
	}
	bool isEnabled() {
		return enabled;
}
