#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "Components.h"
#include "Game.h"
#include "SDL.h"
#include "TextureManager.h"


//See ECS.h
class SpriteComponent : public Component {

	private:
		
		//These are the components that sprites need
		TransformComponent *transform;
		SDL_Texture *texture;
		SDL_Rect srcRect;
		SDL_Rect destRect;

		bool animated = false;
		bool rotated = false;
		double angle = 0;
		int nFrames = 0;
		int oneThousandoverFPS = 100;
		
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

		SpriteComponent(const char* path, double angle, int nFrames, int delay) {
			animated = true;
			this->nFrames = nFrames;
			oneThousandoverFPS = delay;
			setTexture(path);
		}

		SpriteComponent(const char* path, double angle, int nFrames, int delay, bool alive) {
			animated = true;
			this->nFrames = nFrames;
			oneThousandoverFPS = delay;
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

			

			if (animated && ( (Game::isAlive) || (transform->tag != "player"))) {
				srcRect.x = srcRect.w * 
				static_cast<int>((SDL_GetTicks() / oneThousandoverFPS) % nFrames);
			}

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
};


#endif
