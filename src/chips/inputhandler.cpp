#include "chips/inputhandler.hpp"

namespace chips
{	
	/* WARNING: The destructor is NEVER CALLED */ 
	inputHandler & inputHandler::instance()
	{
		static inputHandler* ins = new inputHandler();
		return *ins;
	}
	
	void inputHandler::update()
	{
		_keystates = SDL_GetKeyboardState(NULL);

		SDL_Event event;

		if(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{				
				printf("SDL_QUIT event detected\n");
				SDL_Quit();
				
				// game::instance()._running = false;

			}
		}
	}

	bool inputHandler::isKeyDown(SDL_Scancode key)
	{
		if(_keystates)
		{
			if(_keystates[key])
			{
				return true;
			}
		}

		return false;

	}
	
} // namespace chips


