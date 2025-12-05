#ifndef keyboardcontroller_h
#define keyboardcontroller_h

#include "Game.h"
#include "GameVariables.h"
#include "Components.h"
#include <cmath>

struct KeyboardController : public Component {

	TransformComponent *transform;
	SpriteComponent *sprite;
	bool dashing = false;
	bool canDash = true;
	double originalSpeed;
	double x;
	double y;
	Uint32 timer;
	Uint32 dashMark;
	Uint32 delayMark;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		timer = Game::currentTime;
		
		if (Game::playerIsAlive) {
			//When a key is pressed down
			const Uint8* state = SDL_GetKeyboardState(NULL);

			x = 0;
			y = 0;

			//Change transform based on what key is held down
			if (state[SDL_SCANCODE_W]) y = -1;
			if (state[SDL_SCANCODE_S]) y = 1;
			if (state[SDL_SCANCODE_A]) x = -1;
			if (state[SDL_SCANCODE_D]) x = 1;
			
			if (state[SDL_SCANCODE_SPACE] && !dashing) {
				if (canDash) {
					originalSpeed = transform->speed;
					transform->speed *= 2;
					
					dashing = true;
					dashMark = timer;

					sprite->Play(PLAYER_DASH, 1);
					canDash = false;
				}
			}

			if ((timer - dashMark > DASH_DURATION) && dashing) {
				delayMark = timer;
				transform->speed = originalSpeed;
				dashing = false;
			}

			if (timer - delayMark > DASH_DELAY) {
				canDash = true;
			}
	
			y *= transform->speed;
			x *= transform->speed;

			if (!dashing) {
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
	}
};


#endif
