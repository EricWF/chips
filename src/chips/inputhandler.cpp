#include "chips/inputhandler.hpp"

#include <SDL2/SDL.h>

namespace chips
{
	inputHandler* inputHandler::ins{nullptr};
	
	inputHandler* inputHandler::instance()
	{
		if(ins == 0)
		{
			ins = new inputHandler();
		}

		return ins;
	}


}
