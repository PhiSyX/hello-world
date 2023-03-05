#include <stdio.h>
#include <stdlib.h>

#include <game.h>

int
main(void)
{
	srand(time(NULL));

	struct Game game = create_game();
	run_game(&game);
	destroy_game(game);

	return EXIT_SUCCESS;
}