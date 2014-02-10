#include "chips/inputhandler.hpp"

#include <SDL2/SDL.h>

namespace chips
{	
	inputHandler & inputHandler::instance()
	{
		static inputHandler ins;
		return ins;
	}
}                                                           // namespace chips
