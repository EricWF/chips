#include "chips/game.hpp"
#include "chips/error.hpp"
#include "chips/resource_manager.hpp"
#include "chips/inputhandler.hpp"
#include <elib/aux.hpp>
#include <utility>
#include <stdio.h>


namespace chips
{

    game::game()
    {
        auto & rh = resource_manager::get();
            
        if(!textureManager::instance().load(
                CHIPS_RES_ROOT "/elephant.png", "asd", rh))
         {
            throw chips_error{"Failed to load resource"};
         }

         //_objs.push_back(new gameObject()->load(new loader(100, 100, 132, 80, "asd")));
         //_objs.push_back(new player()->load(new loader(300, 300, 132, 80, "asd")));

        SDL_SetRenderDrawColor(rh, 255, 255, 255, 255);
        _running = true;
    }

	////////////////////////////////////////////////////////////////////////////
	//
	void game::render()
	{
        auto & rh = resource_manager::get();
		SDL_RenderClear(rh);

		for(auto & obj : _objs)
		{
			obj->draw(rh);
		}

		SDL_RenderPresent(rh);
	}
    
	////////////////////////////////////////////////////////////////////////////
	//
	void game::update()
	{
		for(auto & obj : _objs)
		{
			obj->update();
		}
	}

	////////////////////////////////////////////////////////////////////////////
	//
	void game::handleEvents()
	{
		inputHandler::instance().update();
	}
    
	////////////////////////////////////////////////////////////////////////////
	//
	game::~game()
	{
	}

	
}                                                           // namespace chips


