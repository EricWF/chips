#include "chips/inputhandler.hpp"

#include <SDL2/SDL.h>

namespace chips
{	
    /* WARNING: The destructor is NEVER CALLED */
	inputHandler & inputHandler::instance()
	{
		static inputHandler* ins = new inputHandler();
		return *ins;
	}
}                                                           // namespace chips
