#ifndef keyboardcontroller_h
#define keyboardcontroller_h

#include "Game.h"
#include "Components.h"
#include <cmath>

class KeyboardController : public Component {

public:
	TransformComponent *transform;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		
		if (Game::isAlive) {

			cout << transform->velocity.x << endl;
			//When a key is pressed down
			const Uint8* state = SDL_GetKeyboardState(NULL);
			double x = 0;
			double y = 0;

			//Change transform based on what key is held down
			if (state[SDL_SCANCODE_W]) y = -1;
			if (state[SDL_SCANCODE_S]) y = 1;
			if (state[SDL_SCANCODE_A]) x = -1;
			if (state[SDL_SCANCODE_D]) x = 1;
			y *= transform->speed;
			x *= transform->speed;

			//This normalizes the vector
			if (x != 0 && y != 0) {
				Vector2D normalizedVector(x * (sqrt(2) / 2.0), y * (sqrt(2) / 2.0));
				transform->velocity.x = normalizedVector.x;
				transform->velocity.y = normalizedVector.y;
			}

			else {
				transform->velocity.x = x;
				transform->velocity.y = y;
			}
		}
	}
};


#endif
