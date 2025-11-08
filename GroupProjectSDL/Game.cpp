#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"

const char* PLAYER_PATH = "Assets/Fish.png";
const char* ENEMY_PATH = "Assets/Shark.png";

const int PLAYER_HEIGHT = 144;
const int PLAYER_WIDTH = 144;
const int ENEMY_HEIGHT = 144;
const int ENEMY_WIDTH = 144;

const int PLAYER_COLLIDER_HEIGHT = 70;
const int PLAYER_COLLIDER_WIDTH = 50;


const double PLAYER_SPEED = 2.5;


//Each new game object must be intialized, updated, and rendered
Map* map;
Manager manager;

//We add a new entity called player
//addEntity() also puts this entity in unique_ptr<Entity>
//so auto keyword is necessary to reference it again
auto& player(manager.addEntity());
auto& wall(manager.addEntity());

/*
* The renderer is set to a null pointer before
* SDL is initialized
*/

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

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


		//map.h and map.cpp
		map = new Map();

		//See ecs.h and the header files for each component
		player.addComponent<TransformComponent>(0, 0, PLAYER_SPEED, PLAYER_HEIGHT, PLAYER_WIDTH, 1);
		player.addComponent<SpriteComponent>(PLAYER_PATH, PLAYER_HEIGHT, PLAYER_WIDTH);
		player.addComponent<KeyboardController>();
		player.addComponent<ColliderComponent>("player", PLAYER_COLLIDER_HEIGHT, PLAYER_COLLIDER_WIDTH);

		//x, y, speed, height, width, scale
		wall.addComponent<TransformComponent>(300, 300, 0, ENEMY_HEIGHT, ENEMY_WIDTH, 1);
		wall.addComponent<SpriteComponent>(ENEMY_PATH, ENEMY_HEIGHT, ENEMY_WIDTH);
		wall.addComponent<ColliderComponent>("enemy", 70, 60, -10, 0);

	}

	//Initialization was not successful
	else {
		isRunning = false;
	}
}

void Game::handleEvents() {

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


	//See ecs.h (manager and entity class)

	manager.refresh();
	manager.update();

	Vector2D currPosition = player.getComponent<TransformComponent>().position;

	int playerTop = player.getComponent<ColliderComponent>().topEdge;

	int playerLeft = player.getComponent<ColliderComponent>().leftEdge;

	int playerRight = player.getComponent<ColliderComponent>().rightEdge;

	int playerBottom = player.getComponent<ColliderComponent>().bottomEdge;

	if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
		wall.getComponent<ColliderComponent>().collider))
	{
		int wallLeft = wall.getComponent<ColliderComponent>().leftEdge;
		int wallRight = wall.getComponent<ColliderComponent>().rightEdge;
		int wallTop= wall.getComponent<ColliderComponent>().topEdge;
		int wallBottom = wall.getComponent<ColliderComponent>().bottomEdge;


		int overlapRight = playerRight - wallLeft;   // overlap if player is to the left of wall
		int overlapLeft = wallRight - playerLeft;   // overlap if player is to the right of wall
		int overlapTop = wallBottom - playerTop;   // overlap if player is below wall
		int overlapBottom = playerBottom - wallTop;   // overlap if player is above wall

		// Only resolve if there is an actual collision
		if (overlapRight > 0 && overlapLeft > 0 && overlapTop > 0 && overlapBottom > 0) {
			
			// Find the smallest overlap axis
			int minOverlap = std::min({ overlapRight, overlapLeft, overlapTop, overlapBottom });

			//This resolve vector determines how much the player should move
			Vector2D resolve(0, 0);

			if (minOverlap == overlapRight)
				resolve = Vector2D(-overlapRight, 0);
			else if (minOverlap == overlapLeft)
				resolve = Vector2D(overlapLeft, 0);
			else if (minOverlap == overlapTop)
				resolve = Vector2D(0, -overlapTop);
			else if (minOverlap == overlapBottom)
				resolve = Vector2D(0, overlapBottom);

			currPosition += resolve;
			player.getComponent<TransformComponent>().velocity = Vector2D(0, 0); 
			cout << "YOUCH!" << endl;
		}
	}

	
	if (playerLeft < 0) {
		Vector2D newVec(0 - playerLeft, 0);
		currPosition += newVec;
	}
	if ((playerRight) > WINDOW_WIDTH) {
		Vector2D newVec(playerRight - WINDOW_WIDTH, 0);
		currPosition -= newVec;
	}

	if (playerTop < 0) {
		Vector2D newVec(0, 0 - playerTop);
		currPosition += newVec;
	}

	if ((playerBottom) > WINDOW_HEIGHT){
		Vector2D newVec(0, playerBottom - WINDOW_HEIGHT);
		currPosition -= newVec;
	}

	player.getComponent<TransformComponent>().position = currPosition;
}

void Game::render() {
	//Clear our game's renderer data member
	SDL_RenderClear(renderer);
	
	//Add stuff to render


	//See Map.cpp
	map->DrawMap();

	//See ecs.h (manager and entity class)
	manager.draw();


	SDL_RenderPresent(renderer);
}

void Game::clean() {
	//This is how we clean up our game!
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game was cleaned!" << endl;
}


