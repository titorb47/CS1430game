#include "Game.h"

//This initializes the game's memory location
Game *game = nullptr;


int main(int argc, char * argv[]) {

	const double frameRate = 60;
	const double deltaTime = 1000 / frameRate;

	//Uint32 instead of unsigned int because that's what SDL_GetTicks() returns
	Uint32 frameStart;
	int frameTime;



	game = new Game();

	game->init("HELLO!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, false);

	/*
	* -> dereferences the pointer and accesses the game object
	* so the object's member functions can be used
	*/

	while (game->running()) {

		frameStart = SDL_GetTicks();

		game -> handleEvents();
		game -> update();
		game -> render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < deltaTime) {
			SDL_Delay(deltaTime - frameTime);
		}
	}

	game->clean();

	return 0;
}