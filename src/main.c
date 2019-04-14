#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"


int main(void)
{

	srand(time(NULL));

	game_t game;
	
	game_init(&game);

	game_run(&game);

}
