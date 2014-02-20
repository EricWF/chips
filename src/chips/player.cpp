#include "chips/player.hpp"

#include <boost/numeric/ublas/assignment.hpp>
#include <vector>
#include <iostream>

namespace chips
{
	void player::draw(SDL_Renderer *renderer)
	{
		gameObject::draw(renderer);
	}

	void player::update()
	{
		_currFrame = (SDL_GetTicks() / 100) % 4;

		_velocity <<= 0, 0;
		handle_input();
		_pos += _velocity;

		
		
	}

	void player::handle_input()
	{

		if(inputHandler::instance().isKeyDown(SDL_SCANCODE_RIGHT))
		{

			_velocity <<= 2, 0;
		}
		
		if(inputHandler::instance().isKeyDown(SDL_SCANCODE_LEFT))
		{
			_velocity <<= -2, 0;
		}
		
		if(inputHandler::instance().isKeyDown(SDL_SCANCODE_UP))
		{
			_velocity <<= 0, -2;
		}
		
		if(inputHandler::instance().isKeyDown(SDL_SCANCODE_DOWN))
		{
			_velocity <<= 0, 2;
		}
		
	}
	
	void player::clean()
	{
		gameObject::clean();
	}
}
