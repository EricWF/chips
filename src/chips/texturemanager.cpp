#include "chips/texturemanager.hpp"
#include <stdio.h>

namespace chips
{

	textureManager* textureManager::tm{nullptr};

	textureManager* textureManager::instance()
	{
		if(!tm)
		{
			tm = new textureManager();
		}
		
		return tm;
	}

	
	bool textureManager::load(std::string fileName, const std::string id, SDL_Renderer* renderer)
	{
		SDL_Surface *tempSurface = IMG_Load(fileName.c_str());

		if(!tempSurface)
		{
			printf("Failed to make surface");
			return false;
		}
		
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		SDL_FreeSurface(tempSurface);
		
		if(texture != 0)
		{
			printf("Success");
			_textureMap[id] = texture;
			return true;
		}

		printf("Failed to make texture");
		return false;

	}

	void textureManager::draw(const std:: string id, const int x, const int y,
							  const int width, const int height,
							  SDL_Renderer* renderer, SDL_RendererFlip flip)
	{
		SDL_Rect srcRect;
		SDL_Rect destRect;

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = destRect.w = width;
		srcRect.h = destRect.h = height;
		destRect.x = x;
		destRect.y = y;

		SDL_RenderCopyEx(renderer, _textureMap[id], &srcRect, &destRect, 0, 0, flip);

	}

	void textureManager::drawFrame(const std::string id, const int x, const int y,
								   const int width, const int height, const int currRow,
								   const int currFrame, SDL_Renderer *renderer,
								   SDL_RendererFlip flip)
	{
		SDL_Rect srcRect;
		SDL_Rect destRect;
		srcRect.x = width * currFrame;
		srcRect.y = height * (currRow - 1);
		srcRect.w = destRect.w = width;
		srcRect.h = destRect.h = height;
		destRect.x = x;
		destRect.y = y;

		SDL_RenderCopyEx(renderer, _textureMap[id], &srcRect, &destRect, 0, 0, flip);

	}
  
}
