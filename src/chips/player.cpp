#include "chips/player.hpp"

namespace chips
{
	void player::draw()
	{
		//gameObject::draw();
		// TODO
	}

	void player::update()
	{
		_x = 10;
		_y = 20;
	}

	void player::clean()
	{
		gameObject::clean();
	}
}
