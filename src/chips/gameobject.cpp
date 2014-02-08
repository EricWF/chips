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
		textureManager::instance()->drawFrame(_id, _x, _y, _w, _h,_currRow, _currFrame, renderer);
	}

	void gameObject::update()
	{
		_x += 1;
	}
}
