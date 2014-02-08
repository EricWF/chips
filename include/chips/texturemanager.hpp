#ifndef CHIPS_TEXTUREMANAGER_HPP
#define CHIPS_TEXTUREMANAGER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <map>

namespace chips
{
	class textureManager
	{
	public:
		~textureManager() noexcept {}
		static textureManager* instance();
		
		
		textureManager(textureManager const &) = delete;
		textureManager & operator=(textureManager &) = delete;

		bool load(std::string, const std::string, SDL_Renderer*);
		void draw(const std::string, const int, const int, const int, const int,
				  SDL_Renderer*, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void drawFrame(const std::string, const int, const int, const int,
					   const int, const int, const int, SDL_Renderer*,
					   SDL_RendererFlip flip = SDL_FLIP_NONE);
		
	private:
		textureManager() noexcept {}
		std::map<std::string, SDL_Texture*> _textureMap;


	};
}

#endif /* CHIPS_TEXTUREMANAGER_HPP */
    
