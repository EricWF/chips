#include "chips/game.hpp"
#include <elib/assert.hpp>
#include <stdio.h>

namespace chips
{

	////////////////////////////////////////////////////////////////////////////
	//
	/* I think most of these errors are unrecoverable, so I'm just going 
     * to move them over to asserts. */
	bool game::init(const char* title, int xpos, int ypos,
					int width, int height, Uint32 flags)
	{

		if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
            ELIB_ASSERT(false, "SDL init failed");
		}
		// else init passed
        
		_window = SDL_CreateWindow(title, xpos, ypos,
								   width, height, flags);
		if(!_window)
		{
			ELIB_ASSERT(false, "Window init failed");
		}
		// else _window
            
		_renderer = SDL_CreateRenderer(_window, -1, 0);
		if (!_renderer)
		{
            ELIB_ASSERT(false, "Renderer creation failed");
		}
		// else _renderer

		 if(!textureManager::instance().load(
                CHIPS_RES_ROOT "/elephant.png", "asd", _renderer))
		 {
		 	printf("Load image failed");
		 	return false;
		 }

		 _objs.push_back(new gameObject(new loader(100, 100, 132, 80, "asd")));
		 _objs.push_back(new player(new loader(300, 300, 132, 80, "asd")));

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


