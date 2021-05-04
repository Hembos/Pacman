#include <iostream>
#include "Game.h"

int main()
{
	const int WIDTH_MAP = 28,
		HEIGHT_MAP = 32,
		SPRITE_SIZE = 27;

	Game game(SPRITE_SIZE, HEIGHT_MAP, WIDTH_MAP);

	game.renderScene();

	return 0;
}