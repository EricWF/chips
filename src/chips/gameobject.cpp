#include "chips/gameobject.hpp"

namespace chips
{
	gameObject::gameObject(const loader *params)
	{
		_x = params->getX();
		_y = params->getY();
		_h = params->getHeight();
		_w = params->getWidth();
		_id = params->getID();
		_currFrame = 1;
		_currRow = 1;
	}

	void gameObject::draw(SDL_Renderer *renderer)
	{
		textureManager::instance().drawFrame(_id, _x, _y, _w, _h,_currRow, _currFrame, renderer);
	}

	void gameObject::update()
	{
		_x += 1;
	}
}
