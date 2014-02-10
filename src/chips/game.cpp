#include "chips/game.hpp"
#include "chips/chips_error.hpp"
#include "chips/renderer.hpp"
#include <elib/aux.hpp>
#include <utility>
#include <stdio.h>

namespace chips
{

    game::game()
    {
        renderer & rend = renderer::instance();
            
        if(!textureManager::instance().load(
                CHIPS_RES_ROOT "/elephant.png", "asd", rend))
         {
            throw chips_error{"Failed to load resource"};
         }

         _objs.push_back(new gameObject(new loader(100, 100, 132, 80, "asd")));
         _objs.push_back(new player(new loader(300, 300, 132, 80, "asd")));

        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        _running = true;
    }

	////////////////////////////////////////////////////////////////////////////
	//
	void game::render()
	{
        renderer & rend = renderer::instance();
		SDL_RenderClear(rend);

		for(auto & obj : _objs)
		{
			obj->draw(rend);
		}

		SDL_RenderPresent(rend);
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
	game::~game()
	{
	}

	
}                                                           // namespace chips


