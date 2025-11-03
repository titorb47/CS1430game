#include "Game.h"
#include "GameObject.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS.h"
#include "Components.h"

const char* PLAYER_PATH = "Assets/Fish.png";
const char* ENEMY_PATH = "Assets/Shark.png";

const int PLAYER_HEIGHT = 144;
const int PLAYER_WIDTH = 144;
const int ENEMY_HEIGHT = 144;
const int ENEMY_WIDTH = 144;

//Each new game object must be intialized, updated, and rendered
GameObject* player;
GameObject* enemy;
Map* map;

Manager manager;

//We add a new entity called newPlayer
//addEntity() also puts this entity in unique_ptr<Entity>
//so auto keyword is necessary to reference it again
auto& newPlayer(manager.addEntity());

/*
* The renderer is set to a null pointer before
* SDL is initialized
*/

SDL_Renderer* Game::renderer = nullptr;

//Constructor
Game::Game(){
	isRunning = false;
	window = NULL;
	count = 0;
}

//Deconstructor;
Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, bool fullscreen) {
	
	//default value of SDL_WINDOW
	int flags = 0;
	
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	
	//If initialization successful
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "ALL SYSTEMS GO!" << endl;

		//See game.h
		window = SDL_CreateWindow(title, xpos, ypos, WINDOW_WIDTH, WINDOW_HEIGHT, flags);

		//Can use window like a bool
		if (window) {
			cout << "Window created!" << endl;
		}

		//SDL_CreateRenderer(window, index, flags) 
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			cout << "Renderer created!" << endl;
		}

		isRunning = true;

	
		//See GameObject.cpp
		player = new GameObject(PLAYER_PATH, 100, 100, PLAYER_HEIGHT, PLAYER_WIDTH);
		enemy = new GameObject(ENEMY_PATH, 300, 300, ENEMY_HEIGHT, ENEMY_WIDTH);
		map = new Map();

		newPlayer.addComponent<PositionComponent>();
		newPlayer.getComponent<PositionComponent>().setPosition(500, 500);

	}

	//Initialization was not successful
	else {
		isRunning = false;
	}
}

void Game::handleEvents() {
	SDL_Event event;

	//Check and retrieve pending events
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}

void Game::update() {
	//This was just used to keep track of frames
	
	/*
	++count;
	cout << count << endl;
	*/


	//See GameObject.cpp
	player -> Update();
	enemy -> Update();
	manager.update();
	cout << newPlayer.getComponent<PositionComponent>().x() << ","
	<< newPlayer.getComponent<PositionComponent>().y() << endl;

}

void Game::render() {
	//Clear our game's renderer data member
	SDL_RenderClear(renderer);
	
	//Add stuff to render


	//See Map.cpp
	map->DrawMap();

	//See GameObject.cpp
	player -> Render();
	enemy -> Render();

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	//This is how we clean up our game!
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game was cleaned!" << endl;
}


