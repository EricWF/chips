#include "chips/gameobject.hpp"

namespace chips
{
	void gameObject::load(const int x, const int y, const int width,
						  const int height, const std::string id)
	{
		_x = x;
		_y = y;
		_w = width;
		_h = height;
		_id = id;

		_currRow = 1;
		_currFrame = 1;
	}

	void gameObject::draw(SDL_Renderer *renderer)
	{
		
	}


}
