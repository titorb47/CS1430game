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
		int srcSize = 144;
		int destSize = 144;
		
	public:
		
		SpriteComponent() = default;
		
		SpriteComponent(const char* path, int srcSize, int destSize) {
			setTexture(path);
			this->srcSize = srcSize;
			this->destSize = destSize;
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
