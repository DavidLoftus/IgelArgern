#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "game.h"

game_t game;

void sigIntHandler(int signal)
{
	printf("\nTerminated, printing leaderboard: \n");

	game_drawboard(&game);
	game_print_leaderboard(&game);

	exit(1);
}

int main(void)
{

	srand(time(NULL));
	
	game_init(&game);

	signal(SIGINT, sigIntHandler);	

	game_run(&game);

}
