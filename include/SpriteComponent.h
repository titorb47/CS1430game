#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "Components.h"
#include "Game.h"
#include "GameVariables.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>


//See ECS.h
class SpriteComponent : public Component {

	private:
		
		//These are the components that sprites need
		TransformComponent *transform = NULL;
		SDL_Texture *texture = NULL;
		SDL_Rect srcRect = {0,0,0,0};
		SDL_Rect destRect = {0,0,0,0};

		bool animated = false;
		bool dying = false;
		map<string, Animation> animations;
		bool rotated = false;
		double angle = 0;
		int nFrames = 1;
		int oneThousandOverFPS = 1;
		
		int animIndex = 0;

		vector<const char*> textures;
		int timer;
		int deathStart;
		
		
	public:

		SpriteComponent() = default;
		
		SpriteComponent(const char* path) {
			setTexture(path);
		}

		SpriteComponent(const char* path, double angle) {
			setTexture(path);
			rotated = true;
			this->angle = angle;
			
		}


		//maps are like dictionaries
		SpriteComponent(vector<const char*> textures, double angle, 
		map<string, Animation> animations) {
			animated = true;
			rotated = true;
			this->animations = animations;
			this->textures = textures;
			this->angle = angle;
			setTexture(textures.at(0));

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

			timer = Game::currentTime;

			if (animated && Game::playerIsAlive) {
				srcRect.x = srcRect.w * 
				static_cast<int>((timer/ oneThousandOverFPS) % nFrames);
			}

			//Reset SrcRect and then play animation only once
			//This assumes the death animation is being played
			else if (transform->tag == PLAYER_TAG && !Game::playerIsAlive) {
				if (!dying) {
					dying = true;
					deathStart = timer;
					srcRect.x = 0;				
				}

				if (timer - deathStart < PLAYER_DEATH_ANIM_DURATION) {
					srcRect.x = srcRect.w *
						static_cast<int>(((timer - deathStart) / oneThousandOverFPS) % nFrames);
				}
				
				
			}



			destRect.x = static_cast<int>(transform->position.x);
			destRect.y = static_cast<int>(transform->position.y);

			destRect.w = transform->width * transform->scale;
			destRect.h = transform->height * transform->scale;

		}

		void draw() override {
			TextureManager::Draw(texture, srcRect, destRect, angle);
			
			/*Uncomment this to see the destRect*/

			//SDL_SetRenderDrawColor(Game::renderer, 0, 0, 255, 255);
			//SDL_RenderFillRect(Game::renderer, &destRect);
		}

		void Play(const string& animName, int index) {
			setTexture(textures.at(index));
			map<string, Animation> animations = this->animations;
			nFrames = animations[animName].frames;
			animIndex = animations[animName].index;
			oneThousandOverFPS = animations[animName].speed;
		}

		int getNFrames() {
			return nFrames;
		}

		int getOneThousandOverFPS() {
			return oneThousandOverFPS;
		}
};


#endif
