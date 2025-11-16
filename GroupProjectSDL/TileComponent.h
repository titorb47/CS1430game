#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include "ECS.h"
#include "TransformComponent.h"
#include "SDL.h"

class TileComponent : public Component {
	public:
		TransformComponent *transform = 0;
		SpriteComponent *sprite = 0;

		SDL_Rect tileRect = {0,0,0,0};
		int tileID;
		const char* path;

		//Tells compiler to use implicit constructor
		TileComponent() = default;

		TileComponent(int x, int y, int w, int h, int id) {
			tileRect.x = x;
			tileRect.y = y;
			tileRect.w = w;
			tileRect.h = h;

			tileID = id;

			switch (tileID) {
			case 0:
				path = "Assets/DarkWater.png";
				break;
			case 1:
				path = "Assets/MediumWater.png";
				break;
			case 2:
				path = "Assets/LightWater.png";
				break;
			case 3:
				path = "Assets/Bubbles.png";
				break;
			default:
				break;
			}
		}

		void init() override{
			cout << "Initializing tile at " << tileRect.x << ", " << tileRect.y << endl;
			
			//x, y, speed, height, width, scale
			entity->addComponent<TransformComponent>(static_cast<double>(tileRect.x), 
				static_cast<double>(tileRect.y), 0, tileRect.w, tileRect.h, 1);
			
			
			transform = &entity->getComponent<TransformComponent>();


			entity->addComponent<SpriteComponent>();
			
			entity->getComponent<SpriteComponent>().setTexture(path);
			
			sprite = &entity->getComponent<SpriteComponent>();

		}
};

#endif
