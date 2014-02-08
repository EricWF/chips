#include "chips/game.hpp"
#include <stdio.h>

namespace chips
{
	////////////////////////////////////////////////////////////////////////////
	//
	bool game::init(const char* title, int xpos, int ypos,
					int width, int height, Uint32 flags)
	{

		if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			printf("SDL init failed");
			return false;
		}
		// else init passed
        
		_window = SDL_CreateWindow(title, xpos, ypos,
								   width, height, flags);
		if(!_window)
		{
			printf("Window init failed");
			return false;
		}
		// else _window
            
		_renderer = SDL_CreateRenderer(_window, -1, 0);
		if (!_renderer)
		{
			printf("Renderer init failed");
			return false;
		}
		// else _renderer

		 if(!textureManager::instance()->load("/Users/carson/dev/chips/res/elephant.png",
		 									 "asd", _renderer))
		 {
		 	printf("Load image failed");
		 	return false;
		 }

		go.load(100, 100, 132, 80, "asd");
		p.load(300, 300, 132, 80, "asd");

		
		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		_running = true;

		return true;
	}
    
	////////////////////////////////////////////////////////////////////////////
	//
	void game::render()
	{
		SDL_RenderClear(_renderer);

		// textureManager::instance()->draw("asd", 0, 0, 132, 80, _renderer);
		// textureManager::instance()->drawFrame("asd", 100, 100, 132, 80, 1,
		// 									  _currFrame, _renderer);
		// textureManager::instance()->drawFrame("asd", 200, 200, 132, 80, 1,
		// 									  _currFrame, _renderer);

		go.draw(_renderer);
		p.draw(_renderer);

		SDL_RenderPresent(_renderer);
	}
    
	////////////////////////////////////////////////////////////////////////////
	//
	void game::update()
	{
		go.update();
		p.update();
	}

	////////////////////////////////////////////////////////////////////////////
	//
	void game::handleEvents()
	{
		SDL_Event event;

		if(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				_running = false;
				break;
			default:
				break;
			}
		}
	}
    
	////////////////////////////////////////////////////////////////////////////
	//
	void game::clean()
	{
		SDL_DestroyWindow(_window);
		SDL_DestroyRenderer(_renderer);
		SDL_Quit();
	}
}                                                           // namespace chips


