#include "chips/player.hpp"

namespace chips
{
	void player::draw(SDL_Renderer *renderer)
	{
		gameObject::draw(renderer);
	}

	void player::update()
	{
		_x -= 1;
	}
	
	void player::clean()
	{
		gameObject::clean();
	}
}
