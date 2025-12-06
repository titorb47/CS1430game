#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include <cstdlib>
#include <string>
#include "GameVariables.h"


bool Game::playerIsAlive = true;
bool Game::isRunning = false;
bool dying = false;
Uint32 Game::currentTime = 0;
int lastSpawn = 0;
int Game::score = 0;

//Each new game object must be intialized, updated, and rendered
//Map* gameMap;
Manager manager;

//We add a new entity called player and map
//addEntity() also puts this entity in unique_ptr<Entity>
//so auto keyword is necessary to reference it again
auto& player(manager.addEntity());
auto& scoreBackground(manager.addEntity());


//See ECS.cpp to understand how the group bitset works
//The type of this enum is set to size_t
enum groupLabels : size_t {
	PLAYERS_GROUP,
	ENEMIES_GROUP,
	MAP_GROUP
};

/*
* The renderer is set to a null pointer before
* SDL is initialized
*/

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
vector<ColliderComponent*> Game::colliders;

TTF_Font* font;
SDL_Texture* fontTexture;

//Constructor
Game::Game(){
	isRunning = false;
	window = NULL;
}

//Deconstructor;
Game::~Game() {}

void Game::runGame() {
	if (isRunning) {
		Uint32 frameStart = SDL_GetTicks();
		frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < DELTA_TIME) {
			SDL_Delay(DELTA_TIME - frameTime);
		}

		handleEvents();
		update();
		render();

	}
}

void Game::init() {

	cout << "ALL SYSTEMS GO!" << endl;

	TTF_Init();
	font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
	if (font == nullptr) {
		cout << "Error: Font could not load!" << endl;
	}
	else {
		fontTexture = TextureManager::LoadFont(*font, "0",
			FONT_R, FONT_G, FONT_B, FONT_A);
	}

	mapTexture = TextureManager::LoadTexture(MAP_PATH);

	//See ecs.h and the header files for each component
	//ALWAYS DEFINE TRANSFORM FIRST, AND COLLIDER BEFORE SPRITE
	player.addComponent<TransformComponent>(500, 850, PLAYER_SPEED, PLAYER_HEIGHT, 
	PLAYER_WIDTH, PLAYER_SCALE);
	player.getComponent<TransformComponent>().setTag(PLAYER_TAG);
	player.addComponent<SpriteComponent>(PLAYER_PATHS, -90, PLAYER_ANIMATIONS);
	player.addComponent<ColliderComponent>(PLAYER_TAG, PLAYER_COLLIDER_HEIGHT, PLAYER_COLLIDER_WIDTH);
	player.addComponent<KeyboardController>();


	//Add player to PLAYER_GROUP
	player.addGroup(PLAYERS_GROUP);

	//Add the background where the score is kept
	scoreBackground.addComponent<TransformComponent>(SCORE_BACKGROUND_X, 
	SCORE_BACKGROUND_Y, 0, SCORE_BACKGROUND_H, SCORE_BACKGROUND_W, 
	FONT_BACKGROUND_SCALE);
	scoreBackground.addComponent<SpriteComponent>(FONT_BACKGROUND_PATH);
	scoreBackground.addGroup(MAP_GROUP);


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

	//This is the timer that spawns our enemies
	// and carries out animations
	currentTime = SDL_GetTicks();

	if (currentTime > GAME_START_DELAY) {
		GAME_STARTED = true;
	}

	score = currentTime;

	//See ecs.h (manager and entity class)
	manager.refresh();
	manager.update();

	//Get the parts of the player's collider component
	Vector2D currPosition = player.getComponent<TransformComponent>().position;
	int playerTop = player.getComponent<ColliderComponent>().topEdge;
	int playerLeft = player.getComponent<ColliderComponent>().leftEdge;
	int playerRight = player.getComponent<ColliderComponent>().rightEdge;
	int playerBottom = player.getComponent<ColliderComponent>().bottomEdge;

	//For every collider in the colliders vector
	for (auto cc : colliders) {
		
		/*	If the there is a collision between a player and something else,
			and the player is not dashing	*/
		
		if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc)
		&& cc->tag != PLAYER_TAG && 
		!player.getComponent<KeyboardController>().dashing)
		{
			//Get the edges of the collider component the player collided with
			int ccLeft = cc->leftEdge;
			int ccRight = cc->rightEdge;
			int ccTop = cc->topEdge;
			int ccBottom = cc->bottomEdge;


			int overlapRight = playerRight - ccLeft;   // overlap if player is to the left of wall
			int overlapLeft = ccRight - playerLeft;   // overlap if player is to the right of wall
			int overlapTop = ccBottom - playerTop;   // overlap if player is below wall
			int overlapBottom = playerBottom - ccTop;   // overlap if player is above wall

			// Only resolve if there is an actual collision
			if (overlapRight > 0 && overlapLeft > 0 && overlapTop > 0 && overlapBottom > 0) {

				// Find the smallest overlap axis
				int minOverlap = min({ overlapRight, overlapLeft, overlapTop, overlapBottom });

				//This resolve vector determines how much the player should move
				Vector2D resolve;
				resolve.Zero();

				/*	Create a vector based on where the player's collider component
					overlapped with the object's collider component so the player
					moves to the edge of the object's collider component */
				if (minOverlap == overlapRight)
					resolve = Vector2D(-overlapRight, 0);
				else if (minOverlap == overlapLeft)
					resolve = Vector2D(overlapLeft, 0);
				else if (minOverlap == overlapTop)
					resolve = Vector2D(0, -overlapTop);
				else if (minOverlap == overlapBottom)
					resolve = Vector2D(0, overlapBottom);

				/* Move the player by this vector and set velocity to 0 so that
				player does not go past collider while touching it */
				
				currPosition += resolve;
				player.getComponent<TransformComponent>().velocity = Vector2D(0 , 0);
			}

			//If the player hits an enemy, stop animations by setting 
			//playerIsAlive to false

			if (cc->tag == "enemy") {
				playerIsAlive = false;
				player.getComponent<SpriteComponent>().Play(PLAYER_DEATH, 2);
			}
		}
	}
	
	/* For collisions against the edges, making the player
	   move back to the edge once they try to go past it */

	//Left Edge
	if (playerLeft < 0) {
		Vector2D newVec(0 - playerLeft, 0);
		currPosition += newVec;
	}

	//Right Edge
	if ((playerRight) > WINDOW_WIDTH) {
		Vector2D newVec(playerRight - WINDOW_WIDTH, 0);
		currPosition -= newVec;
	}

	//Top Edge
	if (playerTop < TOP) {
		Vector2D newVec(0, TOP - playerTop);
		currPosition += newVec;
	}

	//BottomEdge
	if ((playerBottom) > WINDOW_HEIGHT){
		Vector2D newVec(0, playerBottom - WINDOW_HEIGHT);
		currPosition -= newVec;
	}

	//currPosition is the player's position
	player.getComponent<TransformComponent>().position = currPosition;


	/*  Spawn jellyfish if the player is still alive and a jellyfish
		hasn't spawned since ENEMY_PAWNRATE_MS  */

	if ( (currentTime - lastSpawn >= ENEMY_SPAWNRATE_MS) && 
	playerIsAlive && GAME_STARTED) {

		//Add an enemy
		auto& enemy(manager.addEntity());

		//Lower the spawn rate after each spawn until
		//the minimum spawn rate is reached
		if (ENEMY_SPAWNRATE_MS > 800 - ENEMY_SPAWNRATE_MODIFIER) {
			ENEMY_SPAWNRATE_MS -= 40;
		}
		
		//Add transform component to enemy
		enemy.addComponent<TransformComponent>(rand() % ENEMY_SPAWNX_RANGE, 
		-100, ENEMY_SPEED, ENEMY_HEIGHT, 
		ENEMY_WIDTH, ENEMY_SCALE);

		TransformComponent& transform = enemy.getComponent<TransformComponent>();

		//Add sprite and collier components to enemy
		enemy.addComponent<SpriteComponent>(ENEMY_PATHS, 180, ENEMY_ANIMATIONS);
		enemy.addComponent<ColliderComponent>(ENEMY_TAG, 
		ENEMY_COLLIDER_HEIGHT, ENEMY_COLLIDER_WIDTH);

		//Play the enemy's swim animation
		enemy.getComponent<SpriteComponent>().Play(ENEMY_SWIM, 0);

		//Add enemy to ENEMY_GROUP
		enemy.addGroup(ENEMIES_GROUP);

		//Make the enemy move down
		transform.velocity += enemyVector;

		//Mark timer to prepare for next spawn
		lastSpawn = currentTime;
	}

	if (playerIsAlive && GAME_STARTED) {
		//Scroll the Map
		MAP_SRC_RECT.y = MAP_SIZE_Y - 20 - 
		( (currentTime / MAP_SCROLL_SPEED) % (MAP_SIZE_Y));

		//Play swimming animation
		if (!player.getComponent<KeyboardController>().dashing) {
			player.getComponent<SpriteComponent>().Play(PLAYER_SWIM, 0);
		}

		//Update Score
		string stringScore = to_string(score / 13);
		const char* score = stringScore.c_str();
		fontTexture = TextureManager::LoadFont(*font,
			score, FONT_R, FONT_G, FONT_B, FONT_A);
	}


}

//Here we add our groups

/* enemies are references
   to the vector of entity pointers */
auto& enemies(manager.GetEntityInGroup(ENEMIES_GROUP));

void Game::render() {

	//Clear our game's renderer data member
	SDL_RenderClear(RENDERER);

	//Render the map
	SDL_RenderCopy(RENDERER, mapTexture, &MAP_SRC_RECT, &MAP_DEST_RECT);

	//Render the player
	player.draw();

	//Render each enemy
	for (auto& e : enemies) {
		if (e->getComponent<TransformComponent>().position.y < 1100) {
			e->draw();
		}
		else {
			e->destroy();

		}
	}

	//Render the score background
	scoreBackground.draw();
	
	//Render the font
	SDL_RenderCopy(RENDERER, fontTexture, NULL, &TEXT_DEST_RECT);

	SDL_RenderPresent(RENDERER);
}

void Game::clean() {
	//This is how we clean up our game!
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game was cleaned!" << endl;
}
