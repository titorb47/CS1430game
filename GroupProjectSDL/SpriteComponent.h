#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "Components.h"
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
		int nFrames = 0;
		int oneThousandoverFPS = 100;
		
	public:
		
		SpriteComponent() = default;
		
		SpriteComponent(const char* path) {
			setTexture(path);
		}

		SpriteComponent(const char* path, int nFrames, int delay) {
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

			transform = &entity->getComponent<TransformComponent>();

			srcRect.x = 0;
			srcRect.y = 0;

			srcRect.w = transform->width;
			srcRect.h = transform->height;

		}

		void update() override {

			if (animated) {
				srcRect.x = srcRect.w * 
				static_cast<int>((SDL_GetTicks() / oneThousandoverFPS) % nFrames);
				cout << srcRect.x << endl;
			}

			destRect.x = static_cast<int>(transform->position.x);
			destRect.y = static_cast<int>(transform->position.y);

			destRect.w = transform->width * transform->scale;
			destRect.h = transform->height * transform->scale;
		}

		void draw() override {
			TextureManager::Draw(texture, srcRect, destRect);
		}
};


#endif
