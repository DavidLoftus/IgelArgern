#include <stdio.h>
#include "game.h"


int main(void)
{
	game_t game;
	
	game_init(&game);

	game_drawboard(&game);

}
