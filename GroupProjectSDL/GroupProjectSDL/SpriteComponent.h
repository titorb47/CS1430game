#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "Components.h"
#include "Game.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>


//See ECS.h
class SpriteComponent : public Component {

	private:
		
		//These are the components that sprites need
		TransformComponent *transform;
		SDL_Texture *texture;
		SDL_Rect srcRect;
		SDL_Rect destRect;
		Animation swimAnimation;
		Animation dashAnimation;
		Animation deathAnimation;

		bool animated = false;
		bool rotated = false;
		double angle = 0;
		int nFrames;
		int oneThousandOverFPS;
		
		const int SWIM_FRAMES = 4;
		const int DASH_FRAMES = 3;
		const int DEATH_FRAMES = 5;

		const int SWIM_ANIM_SPEED= 100;
		const int DASH_ANIM_SPEED = 50;
		const int DEATH_ANIM_SPEED = 200;
		
	public:
		
		int animIndex = 0;
		const string SWIM_ANIMATION_NAME = "Swim";
		const string DASH_ANIMATION_NAME = "Dash";
		const string DEATH_ANIMATION_NAME = "Death";


		//maps are like dictionaries
		map<string, Animation> animations;

		SpriteComponent() = default;
		
		SpriteComponent(const char* path) {
			setTexture(path);
		}

		SpriteComponent(const char* path, double angle) {
			setTexture(path);
			rotated = true;
			this->angle = angle;
			
		}

		SpriteComponent(const char* path, double angle, bool isAnimated) {
			animated = isAnimated;
			rotated = true;
			swimAnimation = Animation(0, SWIM_FRAMES, SWIM_ANIM_SPEED);
			dashAnimation = Animation(1, DASH_FRAMES, DASH_ANIM_SPEED);
			deathAnimation = Animation(2, DEATH_FRAMES, DEATH_ANIM_SPEED);

			animations.emplace(SWIM_ANIMATION_NAME, swimAnimation);
			animations.emplace(DASH_ANIMATION_NAME, dashAnimation);
			animations.emplace(DEATH_ANIMATION_NAME, deathAnimation);

			Play(SWIM_ANIMATION_NAME);
			setTexture(path);
		}


		//Destroy the texture upon deconstruction
		~SpriteComponent() {
			SDL_DestroyTexture(texture);
		}


		void setTexture(const char* path) {
			texture = TextureManager::LoadTexture(path);
		}


		void init() override {

			//& gets a copy of the entity
			transform = &entity->getComponent<TransformComponent>();
			

			srcRect.x = 0;
			srcRect.y = 0;

			srcRect.w = transform->width;
			srcRect.h = transform->height;

		}

		void update() override {

			

			if (animated && ( (Game::playerIsAlive) || (transform->tag != "player"))) {
				srcRect.x = srcRect.w * 
				static_cast<int>((SDL_GetTicks() / oneThousandOverFPS) % nFrames);
			}

			srcRect.y = animIndex * transform->height;


			destRect.x = static_cast<int>(transform->position.x);
			destRect.y = static_cast<int>(transform->position.y);

			destRect.w = transform->width * transform->scale;
			destRect.h = transform->height * transform->scale;

		}

		void draw() override {
			if (!rotated) {
				TextureManager::Draw(texture, srcRect, destRect);
			}
			else {
				TextureManager::DrawAngle(texture, srcRect, destRect, angle);
			}
		}

		void Play(const string& animName) {
			nFrames = animations[animName].frames;
			animIndex = animations[animName].index;
			oneThousandOverFPS = animations[animName].speed;
		}
};


#endif
