#include "chips/player.hpp"

namespace chips
{
	void player::load(const int x, const int y, const int width,
					  const int height, const std::string id)
	{
		gameObject::load(x, y, width, height, id);
	}

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
