#include "chips/game.hpp"
#include <stdio.h>

namespace chips
{
	game* game::theGame{nullptr};

	game* game::instance()
	{
		if(!theGame)
		{
			theGame = new game();
		}
		
		return theGame;
	}

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

		 loader *l1 = new loader(100, 100, 132, 80, "asd");
		 loader *l2 = new loader(300, 300, 132, 80, "asd");
		 
		 go = new gameObject(l1);
		 p  = new player(l2);

		 _objs.push_back(go);
		 _objs.push_back(p);

		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		_running = true;

		return true;
	}
    
	////////////////////////////////////////////////////////////////////////////
	//
	void game::render()
	{
		SDL_RenderClear(_renderer);

		for(auto obj : _objs)
		{
			obj->draw(_renderer);
		}

		SDL_RenderPresent(_renderer);
	}
    
	////////////////////////////////////////////////////////////////////////////
	//
	void game::update()
	{
		for(auto obj : _objs)
		{
			obj->update();
		}
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


