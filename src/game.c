#include "game.h"

void game_init(game_t* game)
{
    printf("Enter the number of players: ");
    scanf("%d", &game->numplayers);

    for(int i = 0; i < game->numplayers; ++i)
    {
        init_player(game, i);
    }
}
