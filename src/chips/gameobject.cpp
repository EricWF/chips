#include "chips/gameobject.hpp"

namespace chips
{
	gameObject::gameObject(const loader *params)
	{
		_pos(0) = params->getX();
		_pos(1) = params->getY();
		_id = params->getID();
		_currFrame = 1;
		_currRow = 1;
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
